#include "GameScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "..\components\Cmp_Sprite.h"
#include "..\components\Cmp_Actor_Physics.h"
#include "..\components\Cmp_Player_Physics.h"
#include "../code/components/Cmp_Waypoint.h"
#include "../code/components/Cmp_Actor_Behaviour.h"

#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> test_NPC;
static std::vector<shared_ptr<Entity>> traffic_NPCs;
static shared_ptr<Entity> wreckingBall;
static vector<shared_ptr<Entity>> chains;

VertexArray line;

//  public domain function by Darel Rex Finley, 2006
//
//  This function expects the passed-in values to be on a scale
//  of 0 to 1, and uses that same scale for the return values.
//
//  See description/examples at alienryderflex.com/hsp.html

void RGBtoHSP(
	double  R, double  G, double  B,
	double* H, double* S, double* P) {

	//  Calculate the Perceived brightness.
	*P = sqrt(R * R * Pr + G * G * Pg + B * B * Pb);

	//  Calculate the Hue and Saturation.  (This part works
	//  the same way as in the HSV/B and HSL systems???.)
	if (R == G && R == B) {
		*H = 0.; *S = 0.; return;
	}
	if (R >= G && R >= B) {   //  R is largest
		if (B >= G) {
			*H = 6. / 6. - 1. / 6. * (B - G) / (R - G); *S = 1. - G / R;
		}
		else {
			*H = 0. / 6. + 1. / 6. * (G - B) / (R - B); *S = 1. - B / R;
		}
	}
	else if (G >= R && G >= B) {   //  G is largest
		if (R >= B) {
			*H = 2. / 6. - 1. / 6. * (R - B) / (G - B); *S = 1. - B / G;
		}
		else {
			*H = 2. / 6. + 1. / 6. * (B - R) / (G - R); *S = 1. - R / G;
		}
	}
	else {   //  B is largest
		if (G >= R) {
			*H = 4. / 6. - 1. / 6. * (G - R) / (B - R); *S = 1. - R / B;
		}
		else {
			*H = 4. / 6. + 1. / 6. * (R - G) / (B - G); *S = 1. - G / B;
		}
	}
}



//  public domain function by Darel Rex Finley, 2006
//
//  This function expects the passed-in values to be on a scale
//  of 0 to 1, and uses that same scale for the return values.
//
//  Note that some combinations of HSP, even if in the scale
//  0-1, may return RGB values that exceed a value of 1.  For
//  example, if you pass in the HSP color 0,1,1, the result
//  will be the RGB color 2.037,0,0.
//
//  See description/examples at alienryderflex.com/hsp.html

void HSPtoRGB(
	double  H, double  S, double  P,
	double* R, double* G, double* B) {

	double  part, minOverMax = 1. - S;

	if (minOverMax > 0.) {
		if (H < 1. / 6.) {   //  R>G>B
			H = 6. * (H - 0. / 6.); part = 1. + H * (1. / minOverMax - 1.);
			*B = P / sqrt(Pr / minOverMax / minOverMax + Pg * part * part + Pb);
			*R = (*B) / minOverMax; *G = (*B) + H * ((*R) - (*B));
		}
		else if (H < 2. / 6.) {   //  G>R>B
			H = 6. * (-H + 2. / 6.); part = 1. + H * (1. / minOverMax - 1.);
			*B = P / sqrt(Pg / minOverMax / minOverMax + Pr * part * part + Pb);
			*G = (*B) / minOverMax; *R = (*B) + H * ((*G) - (*B));
		}
		else if (H < 3. / 6.) {   //  G>B>R
			H = 6. * (H - 2. / 6.); part = 1. + H * (1. / minOverMax - 1.);
			*R = P / sqrt(Pg / minOverMax / minOverMax + Pb * part * part + Pr);
			*G = (*R) / minOverMax; *B = (*R) + H * ((*G) - (*R));
		}
		else if (H < 4. / 6.) {   //  B>G>R
			H = 6. * (-H + 4. / 6.); part = 1. + H * (1. / minOverMax - 1.);
			*R = P / sqrt(Pb / minOverMax / minOverMax + Pg * part * part + Pr);
			*B = (*R) / minOverMax; *G = (*R) + H * ((*B) - (*R));
		}
		else if (H < 5. / 6.) {   //  B>R>G
			H = 6. * (H - 4. / 6.); part = 1. + H * (1. / minOverMax - 1.);
			*G = P / sqrt(Pb / minOverMax / minOverMax + Pr * part * part + Pg);
			*B = (*G) / minOverMax; *R = (*G) + H * ((*B) - (*G));
		}
		else {   //  R>B>G
			H = 6. * (-H + 6. / 6.); part = 1. + H * (1. / minOverMax - 1.);
			*G = P / sqrt(Pr / minOverMax / minOverMax + Pb * part * part + Pg);
			*R = (*G) / minOverMax; *B = (*G) + H * ((*R) - (*G));
		}
	}
	else {
		if (H < 1. / 6.) {   //  R>G>B
			H = 6. * (H - 0. / 6.); *R = sqrt(P * P / (Pr + Pg * H * H)); *G = (*R) * H; *B = 0.;
		}
		else if (H < 2. / 6.) {   //  G>R>B
			H = 6. * (-H + 2. / 6.); *G = sqrt(P * P / (Pg + Pr * H * H)); *R = (*G) * H; *B = 0.;
		}
		else if (H < 3. / 6.) {   //  G>B>R
			H = 6. * (H - 2. / 6.); *G = sqrt(P * P / (Pg + Pb * H * H)); *B = (*G) * H; *R = 0.;
		}
		else if (H < 4. / 6.) {   //  B>G>R
			H = 6. * (-H + 4. / 6.); *B = sqrt(P * P / (Pb + Pg * H * H)); *G = (*B) * H; *R = 0.;
		}
		else if (H < 5. / 6.) {   //  B>R>G
			H = 6. * (H - 4. / 6.); *B = sqrt(P * P / (Pb + Pr * H * H)); *R = (*B) * H; *G = 0.;
		}
		else {   //  R>B>G
			H = 6. * (-H + 6. / 6.); *R = sqrt(P * P / (Pr + Pb * H * H)); *B = (*R) * H; *G = 0.;
		}
	}
}




void GameScene::Load()
{
	float tileSize = 50.f;

	//ls::LoadLevelFile_OLD("res/levels/pacman.txt", 50.f);
	ls::LoadLevelFile("res/levels/test_large_embeddedTileset.tmj", this, tileSize);

	// TODO: Remove this debugging to visualise waypoints before release.
	// Spawn a green circle on top of every waypoint for debugging purposes.
	//for (auto& waypoint : ls::GetWaypoints())
	//{
	//	// Create a component
	//	auto waypoint_shape = waypoint.second->addComponent<ShapeComponent>();
	//	float waypoint_trigger_radius = waypoint.second->getComponents<AIWaypointComponent>()[0]->trigger_radius;
	//	waypoint_shape->setShape<sf::CircleShape>(waypoint_trigger_radius);
	//	waypoint_shape->getShape().setFillColor(Color::Green);
	//	waypoint_shape->getShape().setOrigin(Vector2f(waypoint_trigger_radius, waypoint_trigger_radius));
	//}

	// Player Setup
	player = MakeEntity();
	player->setPosition(sf::Vector2f(2.f * tileSize, -5.0f * tileSize));

	// Player Shape Component
	auto shapeComp = player->addComponent<ShapeComponent>();
	sf::Vector2f size = Vector2f(10.f, 30.f);
	shapeComp->setShape<sf::RectangleShape>(size);
	shapeComp->getShape().setFillColor(Color::Magenta);
	shapeComp->getShape().setOrigin(Vector2f(5.f, 15.f));

	for (int i = 0; i < 3; i++)
	{
		// Chain Setup
		auto chain = MakeEntity();
		chain->setPosition(Vector2f(player->getPosition().x, player->getPosition().y + 25.f + i * 20.f));

		// Chain Shape Component
		auto chainShape = chain->addComponent<ShapeComponent>();
		Vector2f size = Vector2f(5.f, 15.f);
		chainShape->setShape<RectangleShape>(size);
		chainShape->getShape().setFillColor(Color::Green);
		chainShape->getShape().setOrigin(size / 2.f);

		// Chain Physics
		auto chainPhysics = chain->addComponent<ActorPhysicsComponent>(true, size);
		chains.push_back(chain);
	}


	// Ball Setup
	wreckingBall = MakeEntity();
	wreckingBall->setPosition(Vector2f(player->getPosition().x, chains[chains.size() - 1]->getPosition().y + 25.f));

	// Ball Shape Component
	auto ballShape = wreckingBall->addComponent<ShapeComponent>();
	float radius = 10.f;
	ballShape->setShape<sf::CircleShape>(radius);
	ballShape->getShape().setFillColor(Color::Red);
	ballShape->getShape().setOrigin(Vector2f(radius, radius));

	// Ball Physics Component
	auto wreckingBallPhysics = wreckingBall->addComponent<ActorPhysicsComponent>(true, radius);

	wreckingBallPhysics->setMass(20.f);
	// Player Physics Component
	auto playerPhysics = player->addComponent<PlayerPhysicsComponent>(size, wreckingBall, chains);
	playerPhysics->setMass(200.f);
	// Camera setup
	PlayerCamera.setCenter(player->getPosition());
	PlayerCamera.zoom(0.8);





	// Make a test NPC
	test_NPC = MakeEntity();
	test_NPC->setPosition(sf::Vector2f(10.f * tileSize, -3.5f * tileSize));
	test_NPC->setRotation(sf::degrees(180));

	// Test NPC Shape Component
	auto shapeCompNPC = test_NPC->addComponent<ShapeComponent>();
	sf::Vector2f sizeNPC = Vector2f(100.f, 45.f);
	shapeCompNPC->setShape<sf::RectangleShape>(sizeNPC);
	shapeCompNPC->getShape().setFillColor(Color::Red);
	shapeCompNPC->getShape().setOrigin(Vector2f(sizeNPC.x / 2, sizeNPC.y / 2));

	// Test NPC Physics Body Component
	// auto physicsCompNPC = test_NPC->addComponent<ActorPhysicsComponent>(true, sizeNPC);

	// Test NPC AI component
	auto AIcompNPC = test_NPC->addComponent<AIBehaviourComponent>();
	// Give them an arbitrary target waypoint to start them off.
	// Tiled starts waypoints indexed from 1, not zero.
	AIcompNPC->waypoint_destination = ls::GetWaypoints().at(3);
	AIcompNPC->waypoint_most_recently_touched = ls::GetWaypoints().at(4);

	// Make extra traffic NPCs
	shared_ptr<ShapeComponent> shapeCompTrafficNPC;
	sf::Vector2f sizeTrafficNPC = Vector2f(100.f, 45.f);
	shared_ptr<AIBehaviourComponent> aiCompTrafficNPC;
	for (auto& waypoint_pair : ls::GetWaypoints())
	{
		float random_chance_of_spawning_NPC = 1.0f;
		if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) > random_chance_of_spawning_NPC) continue;

		auto& waypoint = waypoint_pair.second;

		// Make a traffic NPC
		shared_ptr<Entity> npc = MakeEntity();
		npc->setPosition(waypoint->getPosition());
		npc->setRotation(sf::degrees((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 360));

		// Test NPC Shape Component
		shapeCompTrafficNPC = npc->addComponent<ShapeComponent>();
		shapeCompTrafficNPC->setShape<sf::RectangleShape>(sizeTrafficNPC);
		double red, green, blue, hue, saturation, lightness;
		hue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		saturation = 0.7;
		lightness = 0.7;
		HSPtoRGB(hue, saturation, lightness, &red, &green, &blue);
		shapeCompTrafficNPC->getShape().setFillColor(Color(red * 255, green * 255, blue * 255));
		shapeCompTrafficNPC->getShape().setOrigin(Vector2f(sizeTrafficNPC.x / 2, sizeTrafficNPC.y / 2));

		// Test NPC Physics Body Component
		// auto physicsCompTrafficNPC = npc->addComponent<ActorPhysicsComponent>(true, sizeTrafficNPC);

		// Test NPC AI component
		aiCompTrafficNPC = npc->addComponent<AIBehaviourComponent>();
		// Give them an arbitrary target waypoint to start them off.
		// Tiled starts waypoints indexed from 1, not zero.
		aiCompTrafficNPC->waypoint_destination = waypoint;
		aiCompTrafficNPC->waypoint_most_recently_touched = waypoint;

		traffic_NPCs.push_back(npc);
	}

}

void GameScene::Unload()
{
	player.reset();
	wreckingBall.reset();
	for (auto& e : chains)
		e.reset();
	ls::Unload();
	Scene::Unload();
}

void GameScene::Render()
{
	ls::Render(Engine::getWindow());
	Engine::getWindow().draw(line);
	Scene::Render();
}

void GameScene::Update(const double& dt)
{
	Vector2f movement = player->getPosition() - PlayerCamera.getCenter();
	PlayerCamera.move(movement * (float)dt * 5.f);
	PlayerCamera.setCenter(player->getPosition());

	line.append(sf::Vertex(player->getPosition()));
	Scene::Update(dt);
}

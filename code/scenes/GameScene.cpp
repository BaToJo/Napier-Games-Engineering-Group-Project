#include "GameScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "..\components\Cmp_Sprite.h"
#include "..\components\Cmp_Actor_Physics.h"
#include "..\components\Cmp_Player_Physics.h"
#include "../code/components/Cmp_Waypoint.h"
#include "../code/components/Cmp_Actor_Behaviour.h"
#include "../lib_engine/System_Physics.h"

#include "../lib_engine/Audio.h"
#include "../game.h"
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> test_NPC;
static std::vector<shared_ptr<Entity>> traffic_NPCs;
static shared_ptr<Entity> wreckingBall;
static vector<shared_ptr<Entity>> chains;
static shared_ptr<Entity> cube;

float npc_traffic_linear_damping = 8.0f;
float npc_traffic_angular_damping = 20.0f;

VertexArray line;

//  public domain function by Darel Rex Finley, 2006
//
//  This function expects the passed-in values to be on a scale
//  of 0 to 1, and uses that same scale for the return values.
//
//  See description/examples at alienryderflex.com/hsp.html

void RGBtoHSP(
	double R, double G, double B,
	double *H, double *S, double *P)
{

	//  Calculate the Perceived brightness.
	*P = sqrt(R * R * Pr + G * G * Pg + B * B * Pb);

	//  Calculate the Hue and Saturation.  (This part works
	//  the same way as in the HSV/B and HSL systems???.)
	if (R == G && R == B)
	{
		*H = 0.;
		*S = 0.;
		return;
	}
	if (R >= G && R >= B)
	{ //  R is largest
		if (B >= G)
		{
			*H = 6. / 6. - 1. / 6. * (B - G) / (R - G);
			*S = 1. - G / R;
		}
		else
		{
			*H = 0. / 6. + 1. / 6. * (G - B) / (R - B);
			*S = 1. - B / R;
		}
	}
	else if (G >= R && G >= B)
	{ //  G is largest
		if (R >= B)
		{
			*H = 2. / 6. - 1. / 6. * (R - B) / (G - B);
			*S = 1. - B / G;
		}
		else
		{
			*H = 2. / 6. + 1. / 6. * (B - R) / (G - R);
			*S = 1. - R / G;
		}
	}
	else
	{ //  B is largest
		if (G >= R)
		{
			*H = 4. / 6. - 1. / 6. * (G - R) / (B - R);
			*S = 1. - R / B;
		}
		else
		{
			*H = 4. / 6. + 1. / 6. * (R - G) / (B - G);
			*S = 1. - G / B;
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
	double H, double S, double P,
	double *R, double *G, double *B)
{

	double part, minOverMax = 1. - S;

	if (minOverMax > 0.)
	{
		if (H < 1. / 6.)
		{ //  R>G>B
			H = 6. * (H - 0. / 6.);
			part = 1. + H * (1. / minOverMax - 1.);
			*B = P / sqrt(Pr / minOverMax / minOverMax + Pg * part * part + Pb);
			*R = (*B) / minOverMax;
			*G = (*B) + H * ((*R) - (*B));
		}
		else if (H < 2. / 6.)
		{ //  G>R>B
			H = 6. * (-H + 2. / 6.);
			part = 1. + H * (1. / minOverMax - 1.);
			*B = P / sqrt(Pg / minOverMax / minOverMax + Pr * part * part + Pb);
			*G = (*B) / minOverMax;
			*R = (*B) + H * ((*G) - (*B));
		}
		else if (H < 3. / 6.)
		{ //  G>B>R
			H = 6. * (H - 2. / 6.);
			part = 1. + H * (1. / minOverMax - 1.);
			*R = P / sqrt(Pg / minOverMax / minOverMax + Pb * part * part + Pr);
			*G = (*R) / minOverMax;
			*B = (*R) + H * ((*G) - (*R));
		}
		else if (H < 4. / 6.)
		{ //  B>G>R
			H = 6. * (-H + 4. / 6.);
			part = 1. + H * (1. / minOverMax - 1.);
			*R = P / sqrt(Pb / minOverMax / minOverMax + Pg * part * part + Pr);
			*B = (*R) / minOverMax;
			*G = (*R) + H * ((*B) - (*R));
		}
		else if (H < 5. / 6.)
		{ //  B>R>G
			H = 6. * (H - 4. / 6.);
			part = 1. + H * (1. / minOverMax - 1.);
			*G = P / sqrt(Pb / minOverMax / minOverMax + Pr * part * part + Pg);
			*B = (*G) / minOverMax;
			*R = (*G) + H * ((*B) - (*G));
		}
		else
		{ //  R>B>G
			H = 6. * (-H + 6. / 6.);
			part = 1. + H * (1. / minOverMax - 1.);
			*G = P / sqrt(Pr / minOverMax / minOverMax + Pb * part * part + Pg);
			*R = (*G) / minOverMax;
			*B = (*G) + H * ((*R) - (*G));
		}
	}
	else
	{
		if (H < 1. / 6.)
		{ //  R>G>B
			H = 6. * (H - 0. / 6.);
			*R = sqrt(P * P / (Pr + Pg * H * H));
			*G = (*R) * H;
			*B = 0.;
		}
		else if (H < 2. / 6.)
		{ //  G>R>B
			H = 6. * (-H + 2. / 6.);
			*G = sqrt(P * P / (Pg + Pr * H * H));
			*R = (*G) * H;
			*B = 0.;
		}
		else if (H < 3. / 6.)
		{ //  G>B>R
			H = 6. * (H - 2. / 6.);
			*G = sqrt(P * P / (Pg + Pb * H * H));
			*B = (*G) * H;
			*R = 0.;
		}
		else if (H < 4. / 6.)
		{ //  B>G>R
			H = 6. * (-H + 4. / 6.);
			*B = sqrt(P * P / (Pb + Pg * H * H));
			*G = (*B) * H;
			*R = 0.;
		}
		else if (H < 5. / 6.)
		{ //  B>R>G
			H = 6. * (H - 4. / 6.);
			*B = sqrt(P * P / (Pb + Pr * H * H));
			*R = (*B) * H;
			*G = 0.;
		}
		else
		{ //  R>B>G
			H = 6. * (-H + 6. / 6.);
			*R = sqrt(P * P / (Pr + Pb * H * H));
			*B = (*R) * H;
			*G = 0.;
		}
	}
}

bool hasBeenLoaded = false;
int most_recent_score_threshold = 0;

void GameScene::Load()
{
	if (!hasBeenLoaded)
	{
		auto window_size = Engine::getWindowSize();
		Engine::WindowResize(window_size.x, window_size.y);
		PlayerCamera.zoom(0.8);
		hasBeenLoaded = true;
	}

	float tileSize = 50.f;
	// ls::LoadLevelFile_OLD("res/levels/pacman.txt", 50.f);
	ls::LoadLevelFile("res/levels/demo.tmj", this, tileSize);

	// TODO: Remove this debugging to visualise waypoints before release.
	// Spawn a green circle on top of every waypoint for debugging purposes.
	// for (auto& waypoint : ls::GetWaypoints())
	//{
	//	// Create a component
	//	auto waypoint_shape = waypoint.second->addComponent<ShapeComponent>();
	//	float waypoint_trigger_radius = waypoint.second->getComponents<AIWaypointComponent>()[0]->trigger_radius;
	//	waypoint_shape->setShape<sf::CircleShape>(waypoint_trigger_radius);
	//	waypoint_shape->getShape().setFillColor(Color::Green);
	//	waypoint_shape->getShape().setOrigin(Vector2f(waypoint_trigger_radius, waypoint_trigger_radius));
	//}

	InputManager::IsControllerConnected();
	// Player Setup
	player = MakeEntity();
	player->setPosition(sf::Vector2f(6.5f * tileSize, 6.0f * tileSize));

	// Player Shape Component
	auto shapeComp = player->addComponent<ShapeComponent>();
	sf::Vector2f size = Vector2f(10.f, 30.f);
	shapeComp->setShape<sf::RectangleShape>(size);
	shapeComp->getShape().setFillColor(Color::Magenta);
	shapeComp->getShape().setOrigin(Vector2f(5.f, 15.f));

	for (int i = 0; i < 2; i++)
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
		chainPhysics->setMass(20.f - (i * 5));
		// chainPhysics->setFriction(.1f);
		chains.push_back(chain);
	}

	// Ball Setup
	wreckingBall = MakeEntity();
	wreckingBall->setPosition(Vector2f(player->getPosition().x, chains[chains.size() - 1]->getPosition().y + 25.f));

	// Ball Shape Component
	auto ballShape = wreckingBall->addComponent<ShapeComponent>();
	float radius = 15.f;
	ballShape->setShape<sf::CircleShape>(radius);
	ballShape->getShape().setFillColor(Color::Red);
	ballShape->getShape().setOrigin(Vector2f(radius, radius));

	// Ball Physics Component
	auto wreckingBallPhysics = wreckingBall->addComponent<ActorPhysicsComponent>(true, radius);

	wreckingBallPhysics->setMass(5.f);
	wreckingBallPhysics->setRestitution(1.f);
	// Player Physics Component
	auto playerPhysics = player->addComponent<PlayerPhysicsComponent>(size, wreckingBall, chains);
	playerPhysics->setMass(20.f);

	// Some boxes to knock around for fun.
	for (int cube_x = 0; cube_x < 2; cube_x++)
	{
		for (int cube_y = 0; cube_y < 2; cube_y++)
		{
			cube = MakeEntity();
			cube->setPosition(Vector2f((-13.f + cube_x) * tileSize, (9.0f + cube_y) * tileSize));
			auto cubeShape = cube->addComponent<ShapeComponent>();
			Vector2f testSize = Vector2f(tileSize, tileSize);
			cubeShape->setShape<RectangleShape>(testSize);
			cubeShape->getShape().setFillColor(sf::Color::Yellow);
			cubeShape->getShape().setOrigin(Vector2f(testSize.x / 2.f, testSize.y / 2.f));

			auto cubePhysics = cube->addComponent<ActorPhysicsComponent>(true, testSize);
			cubePhysics->setMass(100.f);
			cubePhysics->getBody()->SetLinearDamping(2.0f);
			cubePhysics->getBody()->SetAngularDamping(2.0f);
			traffic_NPCs.push_back(cube);
		}
	}


	// Camera setup
	PlayerCamera.setCenter(player->getPosition());

	//// Make a test NPC
	// test_NPC = MakeEntity();
	// test_NPC->setPosition(sf::Vector2f(10.f * tileSize, -3.5f * tileSize));
	// test_NPC->setRotation(sf::degrees(180));

	//// Test NPC Shape Component
	// auto shapeCompNPC = test_NPC->addComponent<ShapeComponent>();
	// sf::Vector2f sizeNPC = Vector2f(100.f, 45.f);
	// shapeCompNPC->setShape<sf::RectangleShape>(sizeNPC);
	// shapeCompNPC->getShape().setFillColor(Color::Red);
	// shapeCompNPC->getShape().setOrigin(Vector2f(sizeNPC.x / 2, sizeNPC.y / 2));

	//// Test NPC Physics Body Component
	// auto physicsCompNPC = test_NPC->addComponent<ActorPhysicsComponent>(true, sizeNPC);
	// physicsCompNPC->setMass(5.f);
	// physicsCompNPC->setRestitution(0.1f);
	//// Test NPC AI component
	// auto AIcompNPC = test_NPC->addComponent<AIBehaviourComponent>();
	//// Give them an arbitrary target waypoint to start them off.
	//// Tiled starts waypoints indexed from 1, not zero.
	// AIcompNPC->waypoint_destination = ls::GetWaypoints().at(3);
	// AIcompNPC->waypoint_most_recently_touched = ls::GetWaypoints().at(4);

	// Make extra traffic NPCs
	shared_ptr<ShapeComponent> shapeCompTrafficNPC;
	sf::Vector2f sizeTrafficNPC = Vector2f(100.f, 45.f);
	shared_ptr<AIBehaviourComponent> aiCompTrafficNPC;
	for (auto &waypoint_pair : ls::GetWaypoints())
	{
		float random_chance_of_spawning_NPC = .3f;
		if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) > random_chance_of_spawning_NPC)
			continue;

		auto &waypoint = waypoint_pair.second;

		// Make a traffic NPC
		shared_ptr<Entity> npc = MakeEntity();
		npc->setPosition(waypoint->getPosition());
		// npc->setRotation(sf::degrees((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 360));

		// Test NPC Shape Component
		shapeCompTrafficNPC = npc->addComponent<ShapeComponent>();
		shapeCompTrafficNPC->setShape<sf::RectangleShape>(sizeTrafficNPC);
		double red, green, blue, hue, saturation, lightness;
		hue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		saturation = 0.7;
		lightness = 0.7;
		HSPtoRGB(hue, saturation, lightness, &red, &green, &blue);
		shapeCompTrafficNPC->getShape().setFillColor(Color(red * 255, green * 255, blue * 255));
		shapeCompTrafficNPC->getShape().setOrigin(Vector2f(sizeTrafficNPC.x / 2, sizeTrafficNPC.y / 2));

		npc->setRotation(sf::degrees(90));

		// Test NPC Physics Body Component
		auto physicsCompTrafficNPC = npc->addComponent<ActorPhysicsComponent>(true, sizeTrafficNPC);
		physicsCompTrafficNPC->getBody()->SetAngularDamping(npc_traffic_angular_damping);
		physicsCompTrafficNPC->getBody()->SetLinearDamping(npc_traffic_linear_damping);

		// Test NPC AI component
		aiCompTrafficNPC = npc->addComponent<AIBehaviourComponent>();
		// Give them an arbitrary target waypoint to start them off.
		// Tiled starts waypoints indexed from 1, not zero.
		aiCompTrafficNPC->waypoint_destination = waypoint;
		aiCompTrafficNPC->waypoint_most_recently_touched = waypoint;

		traffic_NPCs.push_back(npc);
	}
	if (_settings[2].second)
	{
		// Authored by rucisko, 2019, https://freesound.org/people/rucisko/sounds/462493/ (creative commons)
		Audio::Music_Load_from_file("res/audio/music_ambience_city_2.ogg", "ambience_city_2");
		Audio::Music_Play("ambience_city_2", 0.4, 1.0);
		// Authored by theplax, 2021, https://freesound.org/people/theplax/sounds/553998/ (creative commons)
		Audio::Music_Load_from_file("res/audio/music_ambience_city_1.ogg", "ambience_city_1");
		Audio::Music_Play("ambience_city_1", 0.2, 1.0);
	}

	if (_settings[3].second)
	{
		// Player car sounds
		// Authored by FreeCarSoundsGaming, 2020, https://freesound.org/people/FreeCarSoundsGaming/sounds/535041/ (creative commons)
		Audio::Sound_Load_from_file("res/audio/sound_engine_rev_loop.ogg", "engine_rev");
		// Authored by C-V, 2021, https://freesound.org/people/C-V/sounds/565597/ (creative commons)
		Audio::Sound_Load_from_file("res/audio/sound_engine_idle_fastest_loop.ogg", "engine_idle");
		// Authored by craigsmith, 2019, https://freesound.org/people/craigsmith/sounds/481668/ (creative commons)
		Audio::Sound_Load_from_file("res/audio/sound_tire_screech_loop.ogg", "sound_tire_screech_loop");
		Audio::Sound_Play_Looping("engine_rev", 0.0f, 1.0f);
		Audio::Sound_Play_Looping("engine_idle", 0.0f, 1.0f);
		Audio::Sound_Play_Looping("sound_tire_screech_loop", 0.0f, 1.0f);

		// Collision impact sounds
		// Authored by Pol, 2017, https://freesound.org/people/P%C3%B3l/sounds/385937/ (creative commons)
		Audio::Sound_Load_from_file("res/audio/sound_collisionSmall_single.ogg", "sound_collisionSmall_single");
		// Authored by qubodup, 2012, https://freesound.org/people/qubodup/sounds/151624/ (creative commons)
		Audio::Sound_Load_from_file("res/audio/sound_collisionBig_single.ogg", "sound_collisionBig_single");
		// Composited from files authored by sandydb (2010, https://freesound.org/people/sandyrb/sounds/95078/) and jsbarrett (2015, https://freesound.org/people/jsbarrett/sounds/321139/)
		Audio::Sound_Load_from_file("res/audio/sound_collisionBiggest_single.ogg", "sound_collisionBiggest_single");

		// Voice announcer, eg. "Multi smash!"
		// Authored by us.
		Audio::Sound_Load_from_file("res/audio/announcer_cantStopSmashing.ogg", "announcer_cantStopSmashing");
		Audio::Sound_Load_from_file("res/audio/announcer_criticalCrash.ogg", "announcer_criticalCrash");
		Audio::Sound_Load_from_file("res/audio/announcer_doubleSmash.ogg", "announcer_doubleSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_holySmash.ogg", "announcer_holySmash");
		Audio::Sound_Load_from_file("res/audio/announcer_humiliationSmash.ogg", "announcer_humiliationSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_hyperSmash.ogg", "announcer_hyperSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_impossibleSmash.ogg", "announcer_impossibleSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_kingSmash.ogg", "announcer_kingSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_ludicrousSmash.ogg", "announcer_ludicrousSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_masterSmash.ogg", "announcer_masterSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_monsterSmash.ogg", "announcer_monsterSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_multiSmash.ogg", "announcer_multiSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_mysteriousSmash.ogg", "announcer_mysteriousSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_oneHitKill.ogg", "announcer_oneHitKill");
		Audio::Sound_Load_from_file("res/audio/announcer_overSmash.ogg", "announcer_overSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_rampage.ogg", "announcer_rampage");
		Audio::Sound_Load_from_file("res/audio/announcer_smashacre.ogg", "announcer_smashacre");
		Audio::Sound_Load_from_file("res/audio/announcer_smashingSpree.ogg", "announcer_smashingSpree");
		Audio::Sound_Load_from_file("res/audio/announcer_smashpocalypse.ogg", "announcer_smashpocalypse");
		Audio::Sound_Load_from_file("res/audio/announcer_smashtacular.ogg", "announcer_smashtacular");
		Audio::Sound_Load_from_file("res/audio/announcer_ultimateSmash.ogg", "announcer_ultimateSmash");
		Audio::Sound_Load_from_file("res/audio/announcer_wickedSmash.ogg", "announcer_wickedSmash");
	}

	Engine::getWindow().setView(PlayerCamera);

	if (_font.loadFromFile("res/fonts/ChakraPetch-Regular.ttf"))
	{
		_score.setFont(_font);
		_score.setFillColor(rustColor);
		_score.setOutlineThickness(1.2);
		_score.setOutlineColor(sf::Color::White);
		_score.setCharacterSize(40);
		_score.setString(std::to_string(_scoreNumber));

		_score.setOrigin(_score.getGlobalBounds().getSize() / 2.f + _score.getLocalBounds().getPosition());
	}
}

void GameScene::Unload()
{
	player.reset();
	wreckingBall.reset();
	cube.reset();
	for (auto &e : chains)
		e.reset();
	// test_NPC.reset();
	for (auto &e : traffic_NPCs)
	{
		e.reset();
	}
	chains.clear();
	Audio::UnloadAll();
	ls::Unload();
	Scene::Unload();
}

void GameScene::Render()
{

	ls::Render(Engine::getWindow());

	Engine::getWindow().draw(line);
	Engine::getWindow().draw(_score);
	Scene::Render();
}

void Announcer(int scoreThreshold, int index, string announcer_line, int scorenumber)
{
	if (scorenumber > scoreThreshold && most_recent_score_threshold < index)
	{
		Audio::Sound_Play(announcer_line, 1.0f, 1.0f);
		most_recent_score_threshold = index;
	}
}

void GameScene::HandleEvents()
{
	Scene::HandleEvents();
}

void GameScene::Update(const double &dt)
{
	Engine::getWindow().setView(PlayerCamera);

	auto ballBody = wreckingBall->getComponents<ActorPhysicsComponent>()[0]->getBody();
	for (b2ContactEdge* edge = ballBody->GetContactList(); edge; edge = edge->next)
	{
		if (edge->contact->IsTouching())
		{
			float32 collided_body_mass = (edge->contact->GetFixtureA()->GetBody()->GetMass());
			if (collided_body_mass >= (float32)1.0f)
			{
				// std::cout << ballBody->GetLinearVelocity().Length() << std::endl;
				edge->contact->GetFixtureA()->GetBody()->SetAngularVelocity(ballBody->GetLinearVelocity().Length());

				auto impact_speed = ballBody->GetLinearVelocity().Length();
				// Increase the score on a hit, with points being exponentially better the higher speed your impact.
				_scoreNumber += impact_speed * impact_speed * dt * 100;

				// Play 3 layered audio collision sounds, adjusting their volume and pitch to scale with 3 different powers of the impact speed. This lets us have small "ding!" sounds for low impact collisons, that transition through a deeper metal impact sound to the biggest smash with broken glass.
				auto audio_impact_strength = impact_speed / 50;
				Audio::Sound_Play("sound_collisionSmall_single", audio_impact_strength, 1 - (audio_impact_strength / 2));
				audio_impact_strength = (impact_speed * impact_speed) / 450;
				Audio::Sound_Play("sound_collisionBig_single", audio_impact_strength, 1 - (audio_impact_strength / 2));
				audio_impact_strength = (impact_speed * impact_speed * impact_speed) / 20000;
				Audio::Sound_Play("sound_collisionBiggest_single", audio_impact_strength, 1 - (audio_impact_strength / 3));
			}
		}
	}

	Announcer(13300, 19, "announcer_cantStopSmashing", _scoreNumber);
	Announcer(12000, 18, "announcer_mysteriousSmash", _scoreNumber);
	Announcer(10800, 17, "announcer_ultimateSmash", _scoreNumber);
	Announcer(9700, 16, "announcer_smashpocalypse", _scoreNumber);
	Announcer(8700, 15, "announcer_smashacre", _scoreNumber);
	Announcer(7800, 14, "announcer_humiliationSmash", _scoreNumber);
	Announcer(6900, 13, "announcer_rampage", _scoreNumber);
	Announcer(6100, 12, "announcer_holySmash", _scoreNumber);
	Announcer(5400, 11, "announcer_monsterSmash", _scoreNumber);
	Announcer(4700, 10, "announcer_wickedSmash", _scoreNumber);
	Announcer(4100, 9, "announcer_masterSmash", _scoreNumber);
	Announcer(3500, 8, "announcer_kingSmash", _scoreNumber);
	Announcer(3000, 7, "announcer_smashingSpree", _scoreNumber);
	Announcer(2500, 6, "announcer_smashtacular", _scoreNumber);
	Announcer(2000, 5, "announcer_impossibleSmash", _scoreNumber);
	Announcer(1600, 4, "announcer_ludicrousSmash", _scoreNumber);
	Announcer(1200, 3, "announcer_hyperSmash", _scoreNumber);
	Announcer(800, 2, "announcer_overSmash", _scoreNumber);
	Announcer(500, 1, "announcer_criticalCrash", _scoreNumber);

	Vector2f movement = player->getPosition() - PlayerCamera.getCenter();
	PlayerCamera.move(movement * (float)dt * 5.f);
	PlayerCamera.setCenter(player->getPosition());

	line.append(sf::Vertex(player->getPosition()));

	sf::Vector2i viewPos = sf::Vector2i(Engine::getWindowSize().x / 2.f,
										80.f);

	sf::Vector2f worldPos = Engine::getWindow().mapPixelToCoords(viewPos);
	_score.setPosition(worldPos);
	_score.setString(std::to_string(_scoreNumber));
	Scene::Update(dt);
}

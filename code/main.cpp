#include <SFML/Graphics.hpp>
#include "LevelSystem.h"
#include "Ecm.h"
#include "Cmp_player_movement.h"
#include "Cmp_Sprite.h"
#include "System_Renderer.h"
#include "Box2D/Box2D.h"

using namespace sf;
using namespace std;    

shared_ptr<Entity> player;
float gameWidth = 800.f;
float gameHeight = 600.f;
View playerCamera;

EntityManager ents;
std::vector<b2Body*> bodies;

b2World* world;

enum _moveState {
    MS_STOP,
    MS_LEFT,
    MS_RIGHT,
    MS_UP,
    MS_DOWN 
};

_moveState moveState = _moveState::MS_STOP;
const Keyboard::Key controls[4]
{
    Keyboard::W,
    Keyboard::A,
    Keyboard::S,
    Keyboard::D
};
#pragma region Box2D Helper Functions
// 1 sfml unit = 30 physics units
const float physics_scale = 30.0f;

// inverse of physics_scale, useful for calculations
const float physics_scale_inv = 1.0f / physics_scale;

// Magic numbers for accuracy of physics simulation
const int32 velocityIterations = 6;
const int32 positionIterations = 2;

//Convert from b2Vec2 to a Vector2f
inline const Vector2f bv2_to_sv2(const b2Vec2& in)
{
    return Vector2f(in.x * physics_scale, (in.y * physics_scale));
}

//Convert from Vector2f to a b2Vec2
inline const b2Vec2 sv2_to_bv2(const Vector2f& in)
{
    return b2Vec2(in.x * physics_scale_inv, (in.y * physics_scale_inv));
}
//Convert from screenspace.y to physics.y (as they are the other way around)
inline const Vector2f invert_height(const Vector2f& in)
{
    return Vector2f(in.x, gameHeight - in.y);
}

//Create a Box2D body with a box fixture
b2Body* CreatePhysicsBox(b2World& World, const bool dynamic, const Vector2f& position, const Vector2f& size)
{
    b2BodyDef BodyDef;
    //Is Dynamic(moving), or static(Stationary)
    BodyDef.type = dynamic ? b2_dynamicBody : b2_staticBody;
    BodyDef.position = sv2_to_bv2(position);
    //Create the body
    b2Body* body = World.CreateBody(&BodyDef);

    //Create the fixture shape
    b2PolygonShape Shape;
    Shape.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
    b2FixtureDef FixtureDef;
    //Fixture properties
    FixtureDef.density = dynamic ? 10.f : 0.f;
    FixtureDef.friction = dynamic ? 0.8f : 1.f;
    FixtureDef.restitution = 1.0;
    FixtureDef.shape = &Shape;
    //Add to body
    body->CreateFixture(&FixtureDef);
    return body;
}

// Create a Box2d body with a box fixture, from a sfml::RectangleShape
b2Body* CreatePhysicsBox(b2World& world, const bool dynamic, const RectangleShape& rs)
{
    return CreatePhysicsBox(world, dynamic, rs.getPosition(), rs.getSize());
}
#pragma endregion

void Load()
{
    // Setting up Physics World and Tiles
    const b2Vec2 gravity(0.f, 0.0f);
    world = new b2World(gravity);
    ls::loadLevelFile("res/levels/pacman.txt", 50.f);

    player = make_shared<Entity>();
    auto s = player->addComponent<ShapeComponent>();
    Vector2f size = Vector2f(10.f, 10.f);
    s->setShape<sf::RectangleShape>(size);
    s->getShape().setFillColor(Color::Yellow);
    s->getShape().setOrigin(Vector2f(12.f, 12.f));

    auto b = CreatePhysicsBox(*world, true, invert_height(player->getPosition()), size);
    bodies.push_back(b);
    ents.list.push_back(player);
}

void Update()
{
    static Clock clock;
    float dt = clock.restart().asSeconds();

    // The view's center is set to the player's position so the camera will center the player.
    Vector2f movement = player->getPosition() - playerCamera.getCenter();
    playerCamera.move(movement * dt * 5.f);

    world->Step(dt, velocityIterations, positionIterations);

    b2Vec2 vel = bodies[0]->GetLinearVelocity();

    if (Keyboard::isKeyPressed(controls[0]))
    {
        moveState = _moveState::MS_UP;
    }
    if (Keyboard::isKeyPressed(controls[1]))
    {
        moveState = _moveState::MS_LEFT;
    }
    if (Keyboard::isKeyPressed(controls[2]))
    {
        moveState = _moveState::MS_DOWN;
    }
    if (Keyboard::isKeyPressed(controls[3]))
    {
        moveState = _moveState::MS_RIGHT;
    }

    b2Vec2 impulse = {0.f, 0.f};
    switch (moveState)
    {
    case MS_LEFT:
        impulse.x = -5;
        break;
    case MS_RIGHT:
        impulse.x = 5;
        break;
    case MS_UP:
        impulse.y = 5;
        break;
    case MS_DOWN:
        impulse.y = -5;
        break;
    }
    b2Vec2 velChange = impulse - vel;
    b2Vec2 impulseToApply = bodies[0]->GetMass() * velChange;
    bodies[0]->ApplyLinearImpulse(impulseToApply, bodies[0]->GetWorldCenter(), true);
    // Sync Position
    player->setPosition(invert_height(bv2_to_sv2(bodies[0]->GetPosition())));
    ents.Update(dt);
}

void Render(RenderWindow& window)
{
    ls::Render(window);
    ents.Render();
    Renderer::Render();
}

int main()
{
    Load();
    RenderWindow window(VideoMode(Vector2u(gameWidth, gameHeight)), "SFML works!");

    // View Setup
    playerCamera.setCenter(Vector2f(0.f, 0.f));
    playerCamera.setSize(Vector2f(500.f, 500.f));

    Renderer::Initialise(window);
        
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }
        window.clear();

        Update();
        // View is set to the window after update (I assume you do camera calculations in the update)
        window.setView(playerCamera);
        Render(window);

        window.display();
    }
    return 0;
}
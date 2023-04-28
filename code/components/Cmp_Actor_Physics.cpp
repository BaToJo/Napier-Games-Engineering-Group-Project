#include "Cmp_Actor_Physics.h"
#include "..\lib_engine\System_Physics.h"
ActorPhysicsComponent::ActorPhysicsComponent(Entity* p, bool dyn, const sf::Vector2f& size) : Component(p), _dynamic(dyn)
{
	// Body Definition
	b2BodyDef BodyDef;
	// Is it dynamic or static? (Maybe Kinetic for player movement?)
	BodyDef.type = _dynamic ? b2_dynamicBody : b2_staticBody;
	BodyDef.position = Physics::Sv2_to_bv2(Physics::Invert_height(p->getPosition()));
	_body = Physics::GetWorld()->CreateBody(&BodyDef);
	_body->SetActive(true);

	// Shape Settings
	b2PolygonShape Shape;
	Shape.SetAsBox(Physics::Sv2_to_bv2(size).x * 0.5f, Physics::Sv2_to_bv2(size).y * 0.5f);

	// Fixture Settings
	b2FixtureDef FixtureDef;
	FixtureDef.friction = _dynamic ? 0.3f : 0.8f;
	FixtureDef.restitution = .2;
	FixtureDef.density = 1.f;
	FixtureDef.shape = &Shape;

	_fixture = _body->CreateFixture(&FixtureDef);
}

ActorPhysicsComponent::ActorPhysicsComponent(Entity* p, bool dyn, const float& radius) : Component(p), _dynamic(dyn)
{
	b2BodyDef BodyDef;
	BodyDef.type = _dynamic ? b2_dynamicBody : b2_staticBody;
	BodyDef.position = Physics::Sv2_to_bv2(Physics::Invert_height(p->getPosition()));
	_body = Physics::GetWorld()->CreateBody(&BodyDef);
	_body->SetActive(true);

	b2CircleShape CircleShape;
	CircleShape.m_radius = radius * Physics::physics_scale_inv;
	b2FixtureDef FixtureDef;
	FixtureDef.friction = _dynamic ? 0.3f : 0.8f;
	FixtureDef.restitution = .2;
	FixtureDef.density = 1.f;
	FixtureDef.shape = &CircleShape;

	_fixture = _body->CreateFixture(&FixtureDef);
}


void ActorPhysicsComponent::Update(double dt)
{
	_parent->setPosition(Physics::Invert_height(Physics::Bv2_to_sv2(_body->GetPosition())));
	_parent->setRotation(sf::degrees(-((180 / b2_pi) * _body->GetAngle())));

}

ActorPhysicsComponent::~ActorPhysicsComponent()
{
	auto a = Physics::GetWorld();
	_body->SetActive(false);
	Physics::GetWorld()->DestroyBody(_body);
	_body = nullptr;
}

// Getters
b2Fixture* const ActorPhysicsComponent::getFixture() const { return _fixture; }

const sf::Vector2f ActorPhysicsComponent::getVelocity() const { return Physics::Bv2_to_sv2(_body->GetLinearVelocity(), true); }

b2Body* const ActorPhysicsComponent::getBody() const
{
	return _body;
}

// Setters
void ActorPhysicsComponent::setRestitution(float r)
{
	_fixture->SetRestitution(r);
}

void ActorPhysicsComponent::setFriction(float f)
{
	_fixture->SetFriction(f);
}

void ActorPhysicsComponent::setMass(float m)
{
	_fixture->SetDensity(m);
}

void ActorPhysicsComponent::setVelocity(const sf::Vector2f& v)
{
	_body->SetLinearVelocity(Physics::Sv2_to_bv2(v, true));
}

// Functions

// This applies the impulse to the centre of the body.
void ActorPhysicsComponent::Impulse(const sf::Vector2f& i)
{
	auto a = b2Vec2(i.x, i.y * -1.f);
	_body->ApplyLinearImpulseToCenter(a, true);
}

void ActorPhysicsComponent::Dampen(const sf::Vector2f& d)
{
	auto vel = _body->GetLinearVelocity();
	vel.x *= d.x;
	vel.y *= d.y;
	_body->SetLinearVelocity(vel);
}








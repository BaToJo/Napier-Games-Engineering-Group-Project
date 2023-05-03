#pragma once
#include "Ecm.h"
#include "../lib/b2d/Box2D/Box2D/Box2D.h"
class ActorPhysicsComponent : public Component
{
protected:
	b2Body* _body;
	const bool _dynamic;
	b2Fixture* _fixture;

public:
	explicit ActorPhysicsComponent(Entity* p, bool dyn, const sf::Vector2f& size);
	explicit ActorPhysicsComponent(Entity* p, bool dyn, const float& radius);
	ActorPhysicsComponent() = delete;

	// Getters
	b2Fixture* const getFixture() const;
	//std::vector<const b2Contact*> getTouching() const;
	const sf::Vector2f getVelocity() const;
	b2Body* const getBody() const;
	// Setters
	void setRestitution(float r);
	void setFriction(float f);
	void setMass(float m);
	void setVelocity(const sf::Vector2f& v);

	// Functions
	void Impulse(const sf::Vector2f& i);
	void Dampen(const sf::Vector2f& d);

	// Overrides
	void Update(double dt) override;
	void Render() {};
	~ActorPhysicsComponent() override;
};
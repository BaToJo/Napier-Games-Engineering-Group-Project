#pragma once
#include <Ecm.h>

class ActorMovementComponent : public Component
{
protected:
	bool validMove(const sf::Vector2f&);
	float _speed;
	
public:
	explicit ActorMovementComponent(Entity* p);
	ActorMovementComponent() = delete;

	float getSpeed() const;
	void setSpeed(float _speed);

	void Move(const sf::Vector2f&);
	void Move(float x, float y);

	void Render() override {}
	void Update(double dt) override;
};
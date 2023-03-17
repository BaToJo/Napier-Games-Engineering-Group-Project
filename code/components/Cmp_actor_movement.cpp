#include "Cmp_actor_movement.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "Cmp_Sprite.h"

using namespace sf;

void ActorMovementComponent::Update(double dt)
{

}

ActorMovementComponent::ActorMovementComponent(Entity* p) : _speed(200.0f), Component(p) {}

bool ActorMovementComponent::validMove(const sf::Vector2f& pos)
{
	return (LevelSystem::getTileAt(pos) != LevelSystem::WALL);
}

// Simple actor component that will validate every move of every movement component.
void ActorMovementComponent::Move(const sf::Vector2f& p)
{
	auto pp = _parent->getPosition() + p;

	//if (validMove(pp))
	//{
		_parent->setPosition(pp);
	//}
}
float ActorMovementComponent::getSpeed() const
{
	return _speed;
}

void ActorMovementComponent::setSpeed(float speed)
{
	_speed = speed;
}
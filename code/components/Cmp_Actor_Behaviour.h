#pragma once
#include "Ecm.h"
#include "../scenes/GameScene.h"

class AIBehaviourComponent : public Component
{
protected:
	sf::Color _defaultColor;
	sf::Angle GetAngularOffset(sf::Vector2f target);
	sf::Angle GetAngularOffset(sf::Angle azimuth);
	void PhysicsDriveForwards(float desired_velocity, double dt);
	void PhysicsTurnToAngle(sf::Angle delta_angle, double dt);

public:

	// The most recently touched waypoint
	std::shared_ptr<Entity> waypoint_most_recently_touched;

	// The destination waypoint
	std::shared_ptr<Entity> waypoint_destination;

	explicit AIBehaviourComponent(Entity* p);
	AIBehaviourComponent() = delete;
	~AIBehaviourComponent() override;

	void MoveToTargetOneTick(sf::Vector2f target, float move_speed, double dt);
	void AimTowardsTargetOneTick(sf::Vector2f target, double dt);
	void AimTowardsTargetOneTick(sf::Angle azimuth, double dt);

	float HowCloseIsNearestCar();

	void Update(double dt) override;
	void Render() {};

};

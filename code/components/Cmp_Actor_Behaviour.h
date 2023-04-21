#pragma once
#include "Ecm.h"

class AIBehaviourComponent : public Component
{
protected:

	// The most recently touched waypoint.
	std::shared_ptr<Entity> _most_recently_touched_waypoint;

	// The destination waypoint.
	std::shared_ptr<Entity> _destination_waypoint;

	void MoveToTargetOneTick(sf::Vector2f target, float move_speed, double dt);
	void AimTowardsTargetOneTick(sf::Vector2f target, double dt);
	void AimTowardsTargetOneTick(sf::Angle azimuth, double dt);
	sf::Angle GetAngularOffset(sf::Vector2f target);
	sf::Angle GetAngularOffset(sf::Angle azimuth);


public:

	// The most recently touched waypoint
	std::shared_ptr<Entity> waypoint_most_recently_touched;

	// The destination waypoint
	std::shared_ptr<Entity> waypoint_destination;

	explicit AIBehaviourComponent(Entity* p, float trigger_radius);
	AIBehaviourComponent() = delete;

	void Update(double dt) override;
};
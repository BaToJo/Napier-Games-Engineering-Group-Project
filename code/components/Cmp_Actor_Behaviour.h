#pragma once
#include "Ecm.h"

class AIBehaviourComponent : public Component
{
protected:
	sf::Angle GetAngularOffset(sf::Vector2f target);
	sf::Angle GetAngularOffset(sf::Angle azimuth);

	struct RayCast_hit
	{
		std::shared_ptr<Entity> actor_hit;
		double distance_to_hit;
	};

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
	RayCast_hit RaycastTo(sf::Angle angle, float range);
	RayCast_hit RaycastTo(sf::Vector2f target, float range);

	void Update(double dt) override;
	void Render() {};

};

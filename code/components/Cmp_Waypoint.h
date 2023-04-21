#pragma once
//#include "Ecm.h"
#include "../lib_ecm/Ecm.h"
// #include "Cmp_Actor_Physics.h"


class AIWaypointComponent : public Component
{
protected:

	// A list of destination waypoints.
	std::vector<std::shared_ptr<Entity>> _destinations;

	// A list of all vehicles currently en-route to this waypoint.
	std::vector<std::shared_ptr<Entity>> _actors_en_route_to_here;

	// A boolean that says whether or not it's only allowed to have one NPC en-route to it at a time.
	bool _allow_multiple_vehicles_to_come_here_simultaneously = false;

	int _id = -1;


public:
	// The trigger radius, which defines the area in which a vehicle is marked as having touched this waypoint.
	float trigger_radius = 10;

	//, std::vector<std::shared_ptr<Entity>> actors_en_route_to_here
	explicit AIWaypointComponent(Entity* p, int id, float trigger_radius, bool allow_multiple_vehicles_to_come_here_simultaneously);
	AIWaypointComponent() = delete;

	void Update(double dt) override;

	std::shared_ptr<Entity> GetRandomDestination();
	void AddDestination(std::shared_ptr<Entity> destination);
	int GetID() const;
};
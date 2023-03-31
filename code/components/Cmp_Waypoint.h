#pragma once
#include "Ecm.h"
class WaypointComponent : public Component
{
protected:
	// List of all legal destination waypoints.
	std::vector<std::shared_ptr<Entity>> _destinations;
	// List of all legal destination waypoints.
	std::vector<std::shared_ptr<Entity>> _vehicles_en_route;
	// The radius that a car is counted as visiting this waypoint if it's inside.
	int _trigger_radius;

	// TODO: Each waypoint should have a target speed that a vehicle should attempt to drive at on its way there. Perhaps this could be derived from the radius of the waypoint, or vice versa.
public:
	WaypointComponent() = delete;
	explicit WaypointComponent(Entity* p);

	void Update(double dt) override;

	void Add_vehicle_en_route(const std::shared_ptr<Entity>& vehicle);

};
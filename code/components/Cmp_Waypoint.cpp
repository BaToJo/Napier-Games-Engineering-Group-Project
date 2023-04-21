#include "Cmp_Waypoint.h"
#include <random>
#include <utility>


AIWaypointComponent::AIWaypointComponent(Entity* p, int id, float _trigger_radius, bool allow_multiple_vehicles_to_come_here_simultaneously) : Component(p)
{
	// _destinations = std::move(destinations);
	// _actors_en_route_to_here = std::move(actors_en_route_to_here);
	_id = id;
	trigger_radius = _trigger_radius;
	_allow_multiple_vehicles_to_come_here_simultaneously = allow_multiple_vehicles_to_come_here_simultaneously;
}

void AIWaypointComponent::Update(double dt)
{
}

std::shared_ptr<Entity> AIWaypointComponent::GetRandomDestination()
{
	// Pick a random index from our list of destinations.
	int index = (rand() % static_cast<int>(_destinations.size()));

	return _destinations[index];
}

void AIWaypointComponent::AddDestination(std::shared_ptr<Entity> destination)
{
	_destinations.push_back(destination);
}

int AIWaypointComponent::GetID() const
{
	return _id;
}


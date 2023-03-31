#include "Cmp_Waypoint.h"

#include "Cmp_NPC_Navigator.h"
#include "..\lib_engine\System_Renderer.h"

void WaypointComponent::Update(double dt)
{
	// If a car (who has this as its destination) is within this waypoint's radius...
	std::vector<int> vehicles_to_remove;
	for (unsigned long long i = 0; i < _vehicles_en_route.size(); ++i)
	{
		if (sf::length(_vehicles_en_route[i]->getPosition() - _parent->getPosition()) < _trigger_radius)
		{
			// Pick a new destination.
			std::shared_ptr<Entity> target_destination = _destinations[((double)rand() / (RAND_MAX + 1)) * (_destinations.size() + 1)];
			// Tell the car to travel there.
			_vehicles_en_route[i]->getComponents<NPCNavigatorComponent>()[0]->MoveTo(target_destination->getPosition());
			// Register the car as en-route to the new destination.
			target_destination->getComponents<WaypointComponent>()[0]->Add_vehicle_en_route(_vehicles_en_route[i]);
			// Remove the vehicle from this waypoint's list of vehicles en-route.
			vehicles_to_remove.push_back(i);
		}
	}
	for (unsigned long long i = 0; i < vehicles_to_remove.size(); ++i)
	{
		_vehicles_en_route.erase(_vehicles_en_route.begin() + i);
		i = i - 1;
	}
}

void WaypointComponent::Add_vehicle_en_route(const std::shared_ptr<Entity>& vehicle)
{
	_vehicles_en_route.push_back(vehicle);
}

WaypointComponent::WaypointComponent(Entity* p) : Component(p) {}
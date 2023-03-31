#pragma once
#include "Ecm.h"
#include <SFML/Graphics/Shape.hpp>

// Box2D: Raycasting functionality
#include "Box2D/Dynamics/b2Fixture.h"

class NPCNavigatorComponent : public Component
{
protected:
	std::shared_ptr<std::vector<std::shared_ptr<Entity>>> _all_vehicles;
	sf::Vector2f _current_destination;

	b2RayCastInput _raycast_input;
	b2RayCastOutput _raycast_output;
	float _raycast_length = 90.0; // This arbitrary default is 3 times the length of the player car.
public:
	NPCNavigatorComponent() = delete;
	explicit NPCNavigatorComponent(Entity* p, std::shared_ptr<std::vector<std::shared_ptr<Entity>>> all_vehicles);

	void Update(double dt) override;

	void MoveTo(sf::Vector2f world_position);
	void SetRaycastLength(double length);

	bool Is_there_a_vehicle_ahead_of_me();

	void Halt();
	void Accelerate_to_Speed(float target_speed);
	void TurnVehicleToAngle(sf::Angle target_angle);
	void TurnVehicleToFacePoint(sf::Vector2f target_point);
};
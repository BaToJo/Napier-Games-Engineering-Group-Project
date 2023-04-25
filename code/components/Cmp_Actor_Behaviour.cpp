#include "Cmp_Actor_Behaviour.h"

#include "Cmp_Actor_Physics.h"
#include "Cmp_Waypoint.h"
#include "Box2D/Collision/b2Collision.h"
#include "Box2D/Dynamics/b2Fixture.h"
#include "../lib_engine/System_Physics.h"
#include "SFML/Graphics/Vertex.hpp"
#include "../scenes/GameScene.h"
#include "../../lib_engine/System_Renderer.h"


//Entity* parent;
sf::Angle angle_to_waypoint_absolute;
sf::RectangleShape debug_line;

AIBehaviourComponent::AIBehaviourComponent(Entity* p) : Component(p)
{
	//parent = _parent;
}

AIBehaviourComponent::~AIBehaviourComponent()
{
}

void AIBehaviourComponent::Update(double dt)
{
	float NPC_move_speed = 230;

	// Raycast in front of the vehicle's direction it wants to travel in.
	RayCast_hit rayCast_hit = RaycastTo(waypoint_destination->getPosition(), 200);
	// RayCast_hit rayCast_hit = RaycastTo(_parent->getRotation(), 1000);
	// RayCast_hit rayCast_hit = RaycastTo(sf::Vector2f(0, 0), 1000);
	std::shared_ptr<Entity> actor_hit = rayCast_hit.actor_hit;
	double rayCast_hit_distance = rayCast_hit.distance_to_hit;


	if (rayCast_hit_distance < (100 * 0.9))
	{
		std::cout << actor_hit->getPosition();
		NPC_move_speed = std::min(NPC_move_speed, (float)rayCast_hit_distance);
	}


	// The NPC attempts to drive directly towards its target waypoint.
	MoveToTargetOneTick(waypoint_destination->getPosition(), NPC_move_speed, dt);

	// When the NPC enters the trigger area for its target waypoint, it decides which waypoint from the list of destination waypoints to set as the next target waypoint.
	sf::Vector2f waypoint_offset = waypoint_destination->getPosition() - _parent->getPosition();
	float waypoint_trigger_radius = waypoint_destination->getComponents<AIWaypointComponent>()[0]->trigger_radius;
	float distance_to_waypoint = abs(waypoint_offset.length());
	if (distance_to_waypoint < waypoint_trigger_radius)
	{
		// Set the current waypoint to a new waypoint.
		std::shared_ptr<Entity> new_waypoint = waypoint_destination->getComponents<AIWaypointComponent>()[0]->GetRandomDestination();
		waypoint_most_recently_touched = waypoint_destination;
		waypoint_destination = new_waypoint;
	}

	// Any waypoint trigger that the NPC activates by being inside it, no matter whether it's the NPC's target or not, will become the new waypoint and the NPC will pick a legal destination.

	// The NPC does not register itself as inside the trigger of the most recent waypoint it's touched.

	// If the NPC's waypoint only allows one vehicle en-route to it and there's already a vehicle en-route, then halt and wait until that vehicle is no longer en-route to it.

}

sf::Angle AIBehaviourComponent::GetAngularOffset(sf::Angle azimuth)
{
	return sf::radians(azimuth.asRadians() - _parent->getRotation().asRadians());
}
sf::Angle AIBehaviourComponent::GetAngularOffset(sf::Vector2f target)
{
	sf::Vector2f offset = target - _parent->getPosition();
	if (offset.lengthSq() > 0)
	{
		return GetAngularOffset(offset.angle());
	}
	return sf::radians(0);
}

void AIBehaviourComponent::AimTowardsTargetOneTick(sf::Angle azimuth, double dt)
{
	// TODO: This is a placeholder that just sets the rotation directly with no physics. This should be updated with proper physics based turning.

	float turning_speed = 80.0f;

	float currentOrientation = _parent->getRotation().asDegrees();

	// Allow wrapping the angle around as sf::Angle doesn't allow negatives.
	float azimuth_degrees = azimuth.asDegrees();
	float wrapped_azimuth_negative = azimuth_degrees - 360;
	float wrapped_azimuth_positive = azimuth_degrees + 360;
	float nearest_azimuth = azimuth_degrees;
	if (abs(wrapped_azimuth_negative - currentOrientation) < abs(azimuth_degrees - currentOrientation))
	{
		nearest_azimuth = wrapped_azimuth_negative;
	}
	else if (abs(wrapped_azimuth_positive - currentOrientation) < abs(azimuth_degrees - currentOrientation))
	{
		nearest_azimuth = wrapped_azimuth_positive;
	}
	angle_to_waypoint_absolute = sf::degrees(abs(nearest_azimuth - currentOrientation));

	// This should turn this vehicle towards the specified azimuth.
	if (nearest_azimuth < currentOrientation)
	{
		sf::Angle newOrientation = sf::degrees(currentOrientation - (turning_speed * dt));
		_parent->setRotation(newOrientation);
	}
	else if (nearest_azimuth > currentOrientation)
	{
		sf::Angle newOrientation = sf::degrees(currentOrientation + (turning_speed * dt));
		_parent->setRotation(newOrientation);
	}
}
AIBehaviourComponent::RayCast_hit AIBehaviourComponent::RaycastTo(sf::Angle angle, float range)
{
	sf::Vector2f vector = sf::Vector2f(cos(angle.asRadians()), sin(angle.asRadians())) + _parent->getPosition();
	return RaycastTo(vector, range);
}
AIBehaviourComponent::RayCast_hit AIBehaviourComponent::RaycastTo(sf::Vector2f target, float range)
{
	// This method was written with reference to the online tutorial https://www.iforce2d.net/b2dtut/raycasting (iforce2d, 2013)

	b2RayCastInput rayCast_input;

	// The position from which we cast the ray.
	rayCast_input.p1 = Physics::Sv2_to_bv2(_parent->getPosition());

	// The target we cast the ray at.
	rayCast_input.p2 = Physics::Sv2_to_bv2(target);

	// We need to take the absolute distance we want to cast out to and translate it into a percentage of the distance between the emitter and target, to give it to Box2D.
	rayCast_input.maxFraction = range / sf::length(target - _parent->getPosition());

	// For each car in the world, cast a ray against it, and remember the closest intersection we've seen from them all.
	float closestIntersection = rayCast_input.maxFraction;
	std::shared_ptr<Entity> actor_hit = nullptr;
	b2Vec2 intersectionNormal(0, 0);
	for (std::shared_ptr<Entity> actor : GameScene::GetAllActors())
	{
		// We don't want to check against ourselves.
		if (actor.get() == _parent) continue;
		auto components = actor->getComponents<ActorPhysicsComponent>();
		// If the entity doesn't have a physics component, ignore it.
		if (components.size() < 1) continue;
		b2Body* body = components[0]->getBody();
		// for (b2Body* body = Physics::GetWorld()->GetBodyList(); body; body = body->GetNext()) {
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {

			b2RayCastOutput rayCast_output;
			if (!f->RayCast(&rayCast_output, rayCast_input, 0 /*This parameter is only used for Box2D chain shapes, which we don't use.*/))
				continue;
			if (rayCast_output.fraction < closestIntersection) {
				closestIntersection = rayCast_output.fraction;
				actor_hit = actor;
				intersectionNormal = rayCast_output.normal;
			}
		}
	}

	double intersection_length = sf::length(Physics::Bv2_to_sv2(closestIntersection * (rayCast_input.p2 - rayCast_input.p1)));

	// TODO: This is for debugging. Disable or remove it before release.
	debug_line = sf::RectangleShape(sf::Vector2f(intersection_length, 4));
	debug_line.setFillColor(sf::Color::White);
	debug_line.setOrigin(sf::Vector2f(0, debug_line.getSize().y / 2));
	debug_line.setPosition(_parent->getPosition());
	debug_line.setRotation((target - _parent->getPosition()).angle());
	// debug_line.setRotation(_parent->getRotation());
	// debug_line.setRotation(sf::degrees(90));
	Renderer::Queue(&debug_line);

	return RayCast_hit{ actor_hit,intersection_length };
}
void AIBehaviourComponent::AimTowardsTargetOneTick(sf::Vector2f target, double dt)
{
	// This should turn this vehicle towards the target location.
	sf::Vector2f offset = target - _parent->getPosition();
	if (offset.lengthSq() > 0)
	{
		AimTowardsTargetOneTick(offset.angle(), dt);
	}
}

void AIBehaviourComponent::MoveToTargetOneTick(sf::Vector2f target, float move_speed, double dt)
{
	// TODO: This is a placeholder that just turns towards the target and moves forwards. This should be updated with proper physics based movement.

	AimTowardsTargetOneTick(target, dt);
	sf::Angle angularOffset = GetAngularOffset(target);
	float distance_to_move = (move_speed * dt) / (1 + angle_to_waypoint_absolute.asRadians());
	sf::Vector2f newPosition = sf::Vector2f(
		distance_to_move * cos(_parent->getRotation().asRadians()),
		distance_to_move * sin(_parent->getRotation().asRadians())
	);
	_parent->setPosition(_parent->getPosition() + newPosition);
}

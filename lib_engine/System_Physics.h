#pragma once
#include <SFML/System/Vector2.hpp>
#include "Box2D/Box2D.h"
namespace Physics 
{
	void Initialise();
	void Shutdown();
	void Update(const double&);

	std::shared_ptr<b2World> GetWorld();

	const float physics_scale = 30.0f;
	const float physics_scale_inv = 1.0f / physics_scale;

	const sf::Vector2f Bv2_to_sv2(const b2Vec2& in, bool scale = true);
	const b2Vec2 Sv2_to_bv2(const sf::Vector2f& in, bool scale = true);
	const sf::Vector2f Invert_height(const sf::Vector2f& in);
} // namespace Physics
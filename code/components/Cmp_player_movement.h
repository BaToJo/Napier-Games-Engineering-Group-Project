#pragma once
#include "Cmp_actor_movement.h"
class PlayerMovementComponent : public ActorMovementComponent
{
public:

	explicit PlayerMovementComponent(Entity* p);
	PlayerMovementComponent() = delete;

	void Update(double dt) override;
	void Render() override;
};
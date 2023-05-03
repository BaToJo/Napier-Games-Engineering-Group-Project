#include "ContactListener.h"
#include <stdio.h>
#include <iostream>
void ContactListener::BeginContact(b2Contact* contact)
{
	std::cout << typeid(contact->GetFixtureA()->GetBody()->GetUserData()).name() << std::endl;
	std::cout << typeid(contact->GetFixtureB()->GetBody()->GetUserData()).name() << std::endl;
}

void ContactListener::EndContact(b2Contact* contact)
{
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}

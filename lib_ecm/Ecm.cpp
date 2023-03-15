#include "Ecm.h"

Entity::Entity() {}

void Entity::Update(double dt)
{
	if (_alive && !_forDeletion)
	{
		for (auto& c : _components)
		{
			if (!c->is_forDeletion())
			{
				c->Update(dt);
			}		
		}
	}

}
void Entity::Render()
{
	if (_visible && !_forDeletion)
	{
		for (auto& c : _components)
		{
			if (!c->is_forDeletion())
			{
				c->Render();
			}
		}
	}
}

const sf::Vector2f& Entity::getPosition() const
{
	return _position;
}

void Entity::setPosition(const sf::Vector2f& pos) 
{ 
	_position = pos; 
}

bool Entity::is_forDeletion() const
{
	return _forDeletion;
}

float Entity::getRotation() const
{
	return _rotation;
}

void Entity::setRotation(float _rotation)
{
	this->_rotation = _rotation;
}

bool Entity::isAlive() const
{
	return _alive;
}

void Entity::setAlive(bool _alive)
{
	this->_alive = _alive;
}

void Entity::setForDelete()
{
	this->_forDeletion = true;
}

bool Entity::isVisible() const
{
	return _visible;
}

void Entity::setVisible(bool _visible)
{
	this->_visible = _visible;
}




Component::Component(Entity* const p) : _parent(p), _forDeletion(false) {}

bool Component::is_forDeletion() const
{
	return _forDeletion;
}

void EntityManager::Update(double dt)
{
	for each (auto& e in list)
	{
		e->Update(dt);
	}
}

void EntityManager::Render()
{
	
	for each (auto& e in list)
	{
		e->Render();
	}
}

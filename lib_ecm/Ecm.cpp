#include "Ecm.h"

Entity::Entity(Scene* const s) : _position({ 0.f, 0.f }), _rotation(0.f), _alive(true), _visible(true), scene(s), _forDeletion(false) {}

Entity::~Entity()
{
// Components can inter-depend on each other, so deleting them may take
// multiple passes. We Keep deleting components until we can't delete any
// more
	int deli = 0;
	while (deli != _components.size()) 
	{
		deli = _components.size();
		_components.erase(
			remove_if(_components.begin(), _components.end(),[](auto& sp) { return (sp.use_count() <= 1); }),
			_components.end());
	}

	if (_components.size() > 0)
	{
		throw std::runtime_error(
			"Can't delete entity, someone is grabbing a component!");
	}

	_components.clear();
}

void Entity::Update(double dt)
{
	if (!_alive)
	{
		return;
	}

	for (size_t i = 0; i < _components.size(); i++)
	{
		if (_components[i]->Is_forDeletion())
		{
			_components.erase(_components.begin() + i);
			i--;
		}

		_components[i]->Update(dt);
	}

}
void Entity::Render()
{
	if (!_visible)
	{
		return;
	}
	for (auto& c : _components)
	{
		c->Render();
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

bool Component::Is_forDeletion() const
{
	return _forDeletion;
}

Component::~Component()
{
}

void EntityManager::Update(double dt)
{
	for (size_t i = 0; i < list.size(); i++) 
	{
		if (list[i]->is_forDeletion()) 
		{
			list.erase(list.begin() + i);
			--i;
			continue;
		}
		if (list[i]->_alive) 
		{
			list[i]->Update(dt);
		}
	}
}

void EntityManager::Render()
{
	
	for(auto& e : list)
	{
		e->Render();
	}
}

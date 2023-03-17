#pragma once
#include "..\lib_maths\maths.h"
#include <algorithm>
#include <memory>
#include <typeindex>
#include <vector>

class Entity;
class Scene;

class Component 
{
	friend Entity;

protected:
	Entity* const _parent;
	bool _forDeletion;
	explicit Component(Entity* const p);

public:
	Component() = delete;

	bool Is_forDeletion() const;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;

	virtual ~Component();

};

struct EntityManager
{
	std::vector<std::shared_ptr<Entity>> list;
	void Update(double dt);
	void Render();
};

class Entity 
{
	friend struct EntityManager;

protected:
	std::vector<std::shared_ptr<Component>> _components;
	sf::Vector2f _position;
	float _rotation;
	bool _alive;       // should be updated
	bool _visible;     // should be rendered
	bool _forDeletion = false; // should be deleted

public:
	Scene* const scene;
	Entity(Scene* const s);
	Entity() = delete;
	virtual ~Entity();
	virtual void Update(double dt);
	virtual void Render();

	const sf::Vector2f& getPosition() const;
	void setPosition(const sf::Vector2f& _position);
	bool is_forDeletion() const;
	float getRotation() const;
	void setRotation(float _rotation);
	bool isAlive() const;
	void setAlive(bool _alive);
	void setForDelete();
	bool isVisible() const;
	void setVisible(bool _visible);


	template <typename T, typename... Targs>
	std::shared_ptr<T> addComponent(Targs... params)
	{
		static_assert(std::is_base_of<Component, T>::value, "T != component");
		std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
		_components.push_back(sp);
		return sp;
	}

	template <typename T>
	const std::vector<std::shared_ptr<T>> getComponents() const
	{
		static_assert(std::is_base_of<Component, T>::value, "T != component");

		std::vector<std::shared_ptr<T>> ret;

		for (auto c : _components)
		{
			auto dd = dynamic_cast<T*>(&(*c));
			if (dd)
			{
				ret.push_back(std::dynamic_pointer_cast<T>(c));
			}
		}

		return std::move(ret);
	}
};



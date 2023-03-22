#include "Cmp_Sprite.h"
#include "..\lib_engine\System_Renderer.h"

void ShapeComponent::Update(double dt)
{
	_shape->setPosition(_parent->getPosition());
	_shape->setRotation(_parent->getRotation());
}

void ShapeComponent::Render() { Renderer::Queue(_shape.get()); };

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p) : Component(p), _shape(std::make_shared<sf::CircleShape>()) {}
#include "Object.h"

Object::Object(Ptr<scene::Scene> scene, std::string const & textureFilename, Recti textureCoords)
{
	_scene = scene;
	_objectOffset = -textureCoords.getSize() / 2;
	_object.set(new flat::Object(_scene));
	_object->setTexture(textureFilename);
	_object->setTextureCoords(textureCoords);
	_object->setZ(1);
	_object->setPosition(_objectOffset);
	_friction = .1f;
	_radius = 32.0f;
	_solid = false;
}

void Object::setVelocity(Vector2f velocity)
{
	_velocity = velocity;
}

void Object::doPhysics(float dt)
{
	if(!_velocity.isZero())
	{
		_velocity += -_velocity.unit() * _friction;
		_object->setPosition(_object->getPosition() + _velocity * dt);
	}
}

void Object::setPosition(Vector2f position)
{
	_object->setPosition(position + _objectOffset);
}


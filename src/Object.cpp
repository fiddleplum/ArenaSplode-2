#include "Object.h"

Object::Object(Type _type, Ptr<kit::scene::Scene> _scene, std::string const & textureFilename, Recti textureCoords)
{
	type = _type;
	scene = _scene;
	object.setNew(scene);
	object->setTexture(textureFilename);
	object->setAsSprite(-textureCoords.getSize() / 2, textureCoords);
	object->setZ(1);
	friction = .999f;
	radius = 32.0f;
	solid = false;
	bounciness = 10;
}

void Object::setBounciness(float _bounciness)
{
	bounciness = _bounciness;
}

void Object::setVelocity(Vector2f _velocity)
{
	velocity = _velocity;
}

void Object::applyImpulse(Vector2f impulse)
{
	velocity += impulse;
}

void Object::doPhysics(float dt)
{
	if(getHeldCharacter().isValid())
	{
		return;
	}
	if(velocity.normSq() > 1.f)
	{
		velocity *= std::powf(1.0f - friction, dt);
	}
	else
	{
		velocity.set(0, 0);
	}
	object->setPosition(object->getPosition() + velocity * dt);
}

void Object::setPosition(Vector2f position)
{
	object->setPosition(position);
}

void Object::setOrientation(float orientation)
{
	object->setOrientation(orientation);
}

void Object::setZ(int z)
{
	object->setZ(z);
}

void Object::update(float dt)
{
}

void Object::onTouch(Ptr<Object> object)
{
}


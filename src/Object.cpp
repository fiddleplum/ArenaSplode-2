#include "Object.h"
#include "Level.h"

Object::Object(Type _type, Ptr<Level> _level, std::string const & textureFilename, Recti textureCoords)
{
	type = _type;
	level = _level;
	friction = .999f;
	radius = 32.0f;
	solid = false;
	bounciness = 1;
	scale = 1.0f;
	object.create(level->getScene());
	object->setTexture(textureFilename);
	if(type == CHARACTER)
	{
		textureCoords.setSize(object->getTexture()->getSize());
		imageScale = 64.0f / object->getTexture()->getSize()[0];
	}
	else
	{
		imageScale = 1;
	}
	object->setAsSprite(-textureCoords.getSize() / 2, textureCoords);
	object->setZ(1);
	object->setScale(scale * imageScale);
}

void Object::setBounciness(float _bounciness)
{
	bounciness = _bounciness;
}

void Object::setScale(float _scale)
{
	scale = _scale;
	if(scale > 4.f)
	{
		scale = 4.f;
	}
	if(scale < .25f)
	{
		scale = .25f;
	}
	object->setScale(scale * imageScale);
}

void Object::setImageScale(float _imageScale)
{
	imageScale = _imageScale;
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
	if(velocity.norm() > 100000.f)
	{
		velocity = velocity.unit() * 10000.f;
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

void Object::preRenderUpdate()
{
}

void Object::onTouch(Ptr<Object> object)
{
}

void Object::onOverTile(Vector2i tilePosition, Vector2f closest)
{
}


#include "Rocket.h"
#include "Level.h"
#include "Explosion.h"
#include "Character.h"
#include <kit/math_util.h>
#include <kit/audio.h>

Rocket::Rocket(int _owned, Ptr<Level> level, int _type)
: Object(ROCKET, level, "art/items.png", Recti::minSize(128 + 32 * _type, 0, 32, 32))
{
	owned = _owned;
	type = _type;
	setSolid(true);
	setRadius(8.f);
	setFriction(0.f);
	willExplode = false;
	explodeCountdown = 0;
	ttl = 10.f;
}

void Rocket::update(float dt)
{
	setOrientation(getOrientation() + .05f);
	if(type == DRUNK)
	{
		setVelocity(getVelocity().rotate2d(kit::math::random(-.2f, +.2f)));
	}
	if(willExplode)
	{
		explodeCountdown -= dt;
		if(explodeCountdown <= 0)
		{
			explode();
		}
	}
	ttl -= dt;
	if(ttl <= 0)
	{
		level->removeObject(this);
	}
}

void Rocket::onTouch(Ptr<Object> object)
{
	if(object->getType() == SWORD && object->getHeldCharacter().isValid() && object->getHeldCharacter()->isSwinging())
	{
		if(getVelocity().dot(object->getPosition() - getPosition()) > 0)
		{
			setVelocity(getVelocity().rotate2d(kit::math::random(-.4f, +.4f)) * -1.f);
		}
	}
	if(type == STRAIGHT || type == DRUNK)
	{
		if(object->getType() == CHARACTER || object->getType() == ROCKET)
		{
			willExplode = true;
			explodeCountdown = 0;
		}
	}
	if(type == CAKE)
	{
		if(object->getType() == CHARACTER || object->getType() == SHELL)
		{
			object->setScale(object->getScale() * 1.2f);
			level->removeObject(this);
		}
	}
}

void Rocket::onOverTile(Vector2i position, Vector2f closest)
{
	Tile const & tile = level->getTile(position);
	if(tile.type == Tile::Wall)
	{
		if(type == STRAIGHT || type == DRUNK)
		{
			willExplode = true;
			explodeCountdown = 0;
		}
		else
		{
			level->removeObject(this);
		}
	}
}

void Rocket::setExplode(float delay)
{
	willExplode = true;
	explodeCountdown = delay;
}

void Rocket::explode()
{
	OwnPtr<Explosion> explosion;
	explosion.create(owned, level);
	explosion->setPosition(getPosition());
	explosion->setScale(getScale());
	level->addObject(explosion);
	level->removeObject(this);
}


#include "Shell.h"
#include "Level.h"
#include "Player.h"
#include "Character.h"
#include <kit/math_util.h>
#include <kit/audio.h>

Shell::Shell(int _owned, Ptr<Level> level, int _type)
: Object(SHELL, level, "art/items.png", Recti::minSize(0 + 48 * _type, 192, 48, 48))
{
	owned = _owned;
	type = _type;
	setSolid(false);
	setRadius(20.f);
	setBounciness(2.0f);
	setFriction(0.f);
	ttl = 20.f;
}

void Shell::setOwned(int _owned)
{
	owned = _owned;
}

void Shell::update(float dt)
{
	Vector2f velocity = getVelocity();
	if(velocity.norm() > 400.f)
	{
		setVelocity(velocity.unit() * 400.f);
	}
	if(!getHeldCharacter().isValid())
	{
		//ttl -= dt;
		if(ttl <= 0)
		{
			level->removeObject(this);
		}
	}
	else
	{
		ttl = 20.f;
	}
}

void Shell::onOverTile(Vector2i tilePosition, Vector2f closest)
{
	if(level->getTile(tilePosition).type == Tile::Wall)
	{
		setRotationSpeed(kit::math::random(-6.0, +6.f));
	}
}

void Shell::onTouch(Ptr<Object> object)
{
	if(type == GREEN)
	{
		if(!getHeldCharacter().isValid())
		{
			if(object->getType() == CHARACTER && object.as<Character>()->getPlayer()->getNumber() != owned && getVelocity().normSq() > 1000.f)
			{
				object.as<Character>()->harm(owned, 2.f * getScale());
			}
			if(object->getType() == CHARACTER || (object->getType() == SWORD && object->getHeldCharacter().isValid() && object->getHeldCharacter()->isSwinging()))
			{
				if(getVelocity().dot(object->getPosition() - getPosition()) > 0)
				{
					setVelocity(getVelocity().rotate2d(kit::math::random(-.4f, +.4f)) * -1.f);
					setRotationSpeed(kit::math::random(-6.0, +6.f));
					object->applyImpulse(-getVelocity());
				}
			}
		}
	}
}


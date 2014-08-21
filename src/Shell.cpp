#include "Shell.h"
#include "Level.h"
#include "Player.h"
#include "Character.h"
#include <kit/math_util.h>
#include <kit/audio.h>

Shell::Shell(int _owned, Ptr<Level> level, int _type)
: Object(SHELL, level, "art/items.png", Recti::minSize(0 + 32 * _type, 192, 32, 32))
{
	owned = _owned;
	type = _type;
	setSolid(false);
	setRadius(12.f);
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

void Shell::onTouch(Ptr<Object> object)
{
	if(type == GREEN)
	{
		if(!getHeldCharacter().isValid())
		{
			if(object->getType() == CHARACTER && object.as<Character>()->getPlayer()->getNumber() != owned && getVelocity().normSq() > 100.f)
			{
				object.as<Character>()->harm(owned, 5.f);
			}
			if(object->getType() == CHARACTER || (object->getType() == SWORD && object->getHeldCharacter().isValid() && object->getHeldCharacter()->isSwinging()))
			{
				if(getVelocity().dot(object->getPosition() - getPosition()) > 0)
				{
					setVelocity(getVelocity().rotate2d(kit::math::random(-.4f, +.4f)) * -1.f);
					object->applyImpulse(-getVelocity());
				}
			}
		}
	}
}


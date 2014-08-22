#include "Explosion.h"
#include "Level.h"
#include "Character.h"
#include "Nuke.h"
#include "Rocket.h"
#include <kit/audio.h>

Explosion::Explosion(int _owned, Ptr<Level> level)
	: Object(EXPLOSION, level, "art/explosion.png", Recti::minSize(0, 0, 128, 128))
{
	owned = _owned;
	setSolid(false);
}

void Explosion::update(float dt)
{
	if(time == 0)
	{
		kit::audio::play("sounds/explosion.wav");
		auto objectsWithin = level->getObjectsWithinCircle(getPosition(), 128.0f * getScale(), this);
		for(auto pair : objectsWithin)
		{
			Ptr<Object> object = pair.first;
			Vector2f impulse = this->getPosition() - object->getPosition();
			if(!impulse.isZero())
			{
				object->applyImpulse(impulse.unit() * -1200.f);
			}
			switch(object->getType())
			{
				case ROCKET:
					object.as<Rocket>()->setExplode(0.1f);
					break;
				case CHARACTER:
				{
					object.as<Character>()->harm(owned, 10.f * getScale());
					break;
				}
				case NUKE:
				{
					object.as<Nuke>()->explode(owned);
				}
			}
		}
	}
	time += dt;
	if(time >= doneExplodingTime)
	{
		level->removeObject(this);
	}
}


#include "Explosion.h"
#include "Level.h"
#include "Character.h"
#include "Rocket.h"
#include <kit/audio.h>

Explosion::Explosion(Ptr<Level> level)
: Object(EXPLOSION, level, "art/explosion.png", Recti::minSize(0, 0, 128, 128))
{
	setSolid(false);
}

void Explosion::update(float dt)
{
	if(time == 0)
	{
		kit::audio::play("sounds/explosion.wav");
		auto objectsWithin = level->getObjectsWithinCircle(getPosition(), 128.0f, this);
		for(auto pair : objectsWithin)
		{
			Ptr<Object> object = pair.first;
			switch(object->getType())
			{
			case ROCKET:
				object.as<Rocket>()->setExplode(0.1f);
				break;
			case CHARACTER:
				object.as<Character>()->harm(10.f);
				break;
			}
		}
	}
	time += dt;
	if(time >= doneExplodingTime)
	{
		level->removeObject(this);
	}
}


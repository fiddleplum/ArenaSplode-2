#include "Nuke.h"
#include "Level.h"
#include "Explosion.h"
#include <kit/math_util.h>
#include <kit/audio.h>

Nuke::Nuke(Ptr<Level> level)
: Object(NUKE, level, "art/items.png", Recti::minSize(128, 128, 48, 48))
{
	setRadius(24.f);
	setSolid(true);
	setFriction(.99f);
	exploding = false;
}

void Nuke::explode(int owned)
{
	if(!exploding)
	{
		exploding = true;
		Vector2f range = Vector2f(level->getSize().scale(level->getTileSize())) * .25f;
		for(int i = 0; i < 10; i++)
		{
			OwnPtr<Explosion> explosion;
			explosion.create(owned, level);
			explosion->setPosition(getPosition() + Vector2f(kit::math::random(-range[0], +range[0]), kit::math::random(-range[1], +range[1])));
			explosion->setScale(10.f * getScale());
			level->addObject(explosion);
		}
		level->removeObject(this);
	}
}


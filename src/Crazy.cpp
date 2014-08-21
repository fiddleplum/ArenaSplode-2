#include "Crazy.h"
#include "Character.h"
#include "Level.h"
#include <kit/app.h>
#include <kit/audio.h>
#include <kit/math_util.h>

Crazy::Crazy(Ptr<Level> level)
: Object(CRAZY, level, "art/items.png", Recti::minSize(32, 192, 16, 48))
{
	setSolid(false);
	setFriction(.99f);
}

void Crazy::onTouch(Ptr<Object> object)
{
	if(object->getType() == CHARACTER)
	{
		object.as<Character>()->setCrazy();
		level->removeObject(this);
	}
}


#include "Shrinker.h"
#include <kit/audio.h>
#include <kit/math_util.h>

Shrinker::Shrinker(Ptr<Level> level)
: Object(SHRINKER, level, "art/items.png", Recti::minSize(0, 64, 64, 64))
{
	setSolid(false);
	setFriction(.99f);
}

void Shrinker::onTouch(Ptr<Object> object)
{
	if(object->getType() == Object::CHARACTER && getHeldCharacter().isValid() && getHeldCharacter() != object)
	{
		Vector2f impulse = (this->getPosition() - object->getPosition()).unit() * 400.f;
		object->applyImpulse(-impulse);
		this->applyImpulse(impulse);
		object->setScale(object->getScale() * .9f);
		object->setFriction(object->getFriction() * .99f);
	}
}


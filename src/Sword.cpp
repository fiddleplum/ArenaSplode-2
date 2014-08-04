#include "Sword.h"
#include "Character.h"
#include <kit/audio.h>
#include <kit/math_util.h>

Sword::Sword(Ptr<Level> level)
: Object(SWORD, level, "art/items.png", Recti::minSize(0, 0, 64, 64))
{
	setSolid(false);
	setFriction(.99f);
}

void Sword::onTouch(Ptr<Object> object)
{
	if(object->getType() == Object::CHARACTER && getHeldCharacter() != object)
	{
		Vector2f impulse = (this->getPosition() - object->getPosition()).unit() * 400.f;
		object->applyImpulse(-impulse);
		this->applyImpulse(impulse);
		object.as<Character>()->harm(15.f);
	}
	else if(object->getType() == Object::SWORD && object->getHeldCharacter().isValid())
	{
		kit::audio::play("sounds/sword" + std::to_string(kit::math::random(0, 3)) + ".ogg");
		Vector2f impulse = (this->getPosition() - object->getPosition()).unit() * 400.f;
		object->applyImpulse(-impulse);
		this->applyImpulse(impulse);
	}
}


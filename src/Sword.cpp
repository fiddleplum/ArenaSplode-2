#include "Sword.h"

Sword::Sword(Ptr<kit::scene::Scene> scene)
: Object(SWORD, scene, "art/items.png", Recti::minSize(0, 0, 64, 64))
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
	}
}


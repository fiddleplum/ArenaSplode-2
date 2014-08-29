#include "Chain.h"
#include "Character.h"
#include "Level.h"
#include <kit/app.h>
#include <kit/audio.h>
#include <kit/math_util.h>

Chain::Chain(Ptr<Character> _character1, Ptr<Character> _character2, Ptr<Level> level)
: Object(CHAIN, level, "art/items.png", Recti::minSize(128, 64, 128, 32))
{
	character1 = _character1;
	character2 = _character2;
	setSolid(false);
	setFriction(.99f);
	startTime = kit::app::getTime();
}

bool Chain::holdsCharacter(Character const * character)
{
	if(character1.raw() == character || character2.raw() == character)
	{
		return true;
	}
	return false;
}

void Chain::onTouch(Ptr<Object> object)
{
	if(object->getType() == CHARACTER)
	{
		object.as<Character>()->setObjectHeld(Ptr<Object>());
	}
}

void Chain::update(float dt)
{
	float k = .04f;
	float s = 256.f;
	Vector2f r = character1->getPosition() - character2->getPosition();
	float d = r.norm();
	if(d > 0)
	{
		r.normalize();
		character1->applyImpulse(r * -(d - s) * k);
		character2->applyImpulse(r * (d - s) * k);
	}

	if(kit::app::getTime() - startTime > 10.f)
	{
		level->removeObject(this);
	}
}

void Chain::preRenderUpdate()
{
	Vector2f r = character1->getPosition() - character2->getPosition();
	setPosition((character1->getPosition() + character2->getPosition()) / 2.f);
	setOrientation(std::atan2(r[1], r[0]));
	setScale(r.norm() / 128.f);
}


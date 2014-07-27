#include "Rocket.h"
#include "Level.h"

Rocket::Rocket(Ptr<Level> level)
: Object(ROCKET, level, "art/items.png", Recti::minSize(128, 0, 16, 16))
{
	setSolid(true);
	setFriction(0.f);
}

void Rocket::onTouch(Ptr<Object> object)
{
}


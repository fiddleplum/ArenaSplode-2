#include "RocketLauncher.h"
#include "Level.h"
#include "Rocket.h"
#include "Character.h"

RocketLauncher::RocketLauncher(Ptr<Level> level)
: Object(ROCKET_LAUNCHER, level, "art/items.png", Recti::minSize(64, 0, 64, 64))
{
	setSolid(false);
	setFriction(.99f);
}

void RocketLauncher::fire()
{
	OwnPtr<Rocket> rocket;
	rocket.setNew(level);
	Vector2f r = (getPosition() - getHeldCharacter()->getPosition()).unit();
	rocket->setPosition(getPosition() + r * (getHeldCharacter()->getRadius() + rocket->getRadius() * 1.2f));
	rocket->setVelocity(r * 400.f);
	level->addObject(rocket);
}


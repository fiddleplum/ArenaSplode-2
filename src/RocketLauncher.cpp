#include "RocketLauncher.h"
#include "Level.h"
#include "Rocket.h"
#include "Character.h"
#include "Player.h"

RocketLauncher::RocketLauncher(Ptr<Level> level, int _type)
: Object(ROCKET_LAUNCHER, level, "art/items.png", Recti::minSize(64, 64 * _type, 64, 64))
{
	type = _type;
	setSolid(false);
	setFriction(.99f);
}

void RocketLauncher::fire()
{
	OwnPtr<Rocket> rocket;
	int rocketType;
	switch(type)
	{
	case STRAIGHT:
		rocketType = Rocket::STRAIGHT; break;
	case SHRINKER:
		rocketType = Rocket::SHRINKER; break;
	case DRUNK:
		rocketType = Rocket::DRUNK; break;
	}
	rocket.create(getHeldCharacter()->getPlayer()->getNumber(), level, rocketType);
	Vector2f r = (getPosition() - getHeldCharacter()->getPosition()).unit();
	rocket->setPosition(getPosition() + r * (getHeldCharacter()->getRadius() + rocket->getRadius() * 1.2f));
	rocket->setVelocity(r * 400.f);
	level->addObject(rocket);
}


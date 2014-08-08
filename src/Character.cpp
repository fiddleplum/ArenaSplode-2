#include "Character.h"
#include "RocketLauncher.h"
#include "Level.h"
#include "Player.h"
#include "Game.h"
#include <kit/math_util.h>
#include <kit/audio.h>

Character::Character(Player * _player, Ptr<Level> level, std::string const & characterFilename)
: Object(CHARACTER, level, "art/characters/" + characterFilename, Recti::minSize(0, 0, 64, 64))
{
	player = _player;

	setSolid(true);
	setZ(3);

	heldOrientationOffset = 0;
	heldRadiusOffset = 0;

	health = 100.f;

	maxSpeed = 5000.f;

	numKills = 0;
	swinging = false;
}

void Character::setObjectHeld(Ptr<Object> object)
{
	if(object.isNull() && objectHeld.isValid())
	{
		objectHeld->setVelocity(getVelocity());
		objectHeld->setZ(1);
	}
	objectHeld = object;
	if(objectHeld.isValid())
	{
		objectHeld->setZ(2);
	}
}

void Character::useHeld()
{
	if(!objectHeld.isValid())
	{
		return;
	}
	switch(objectHeld->getType())
	{
	case SWORD:
		swinging = true;
		heldOrientationOffset = (float)kit::math::PI_OVER_4;
		heldRadiusOffset = getRadius() * .5f;
		break;
	case ROCKET_LAUNCHER:
		objectHeld.as<RocketLauncher>()->fire();
		break;
	}
}

void Character::harm(int owned, float amount)
{
	if(owned != -1 && player->getNumber() != owned)
	{
		game->players[owned]->addScore(1);
	}
	int r = kit::math::random(0, 3);
	kit::audio::play("sounds/hurt" + std::to_string(r) + ".ogg");
	health -= amount;
	if(health < 0)
	{
		if(owned != -1 && player->getNumber() != owned)
		{
			game->players[owned]->addScore(10);
		}
		die();
	}
}

void Character::die()
{
	int r = kit::math::random(0, 3);
	kit::audio::play("sounds/death" + std::to_string(r) + ".ogg");
	setObjectHeld(Ptr<Object>());
	level->removeObject(this);
	player->spawnNewCharacter();
}

void Character::incNumKills()
{
	numKills++;
	if(numKills > 5)
	{
		kit::audio::play("sounds/multikill.ogg");
	}
}

void Character::update(float dt)
{
	if(getVelocity().norm() > maxSpeed)
	{
		setVelocity(getVelocity().unit() * maxSpeed);
	}
	if(getScale() < 1.f)
	{
		setScale(.001f + .999f * getScale());
		setFriction(.999f * getScale() + .7f * (1.f - getScale()));
		if(getScale() > 1.f)
		{
			setScale(1.f);

		}
	}
	if(swinging)
	{
		heldOrientationOffset -= (float)kit::math::TWO_PI * dt;
		if(heldOrientationOffset < -kit::math::PI_OVER_4)
		{
			swinging = false;
			heldOrientationOffset = 0;
			heldRadiusOffset = 0;
		}
	}
}

void Character::preRenderUpdate()
{
	if(objectHeld.isValid())
	{
		objectHeld->setPosition(getPosition() + (getRadius() + heldRadiusOffset) * Vector2f(std::cosf(getOrientation() + heldOrientationOffset), std::sinf(getOrientation() + heldOrientationOffset)));
		objectHeld->setOrientation(getOrientation() + heldOrientationOffset);
	}
}


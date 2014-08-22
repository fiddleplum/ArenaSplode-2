#include "Character.h"
#include "RocketLauncher.h"
#include "Level.h"
#include "Shell.h"
#include "Nuke.h"
#include "Player.h"
#include "Game.h"
#include "RobotBody.h"
#include <kit/app.h>
#include <kit/math_util.h>
#include <kit/audio.h>

Character::Character(Player * _player, Ptr<Level> level, std::string const & characterFilename)
	: Object(CHARACTER, level, "art/characters/" + characterFilename, Recti::minSize(0, 0, 64, 64))
{
	filename = characterFilename;
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
	if(!object.isValid() && objectHeld.isValid())
	{
		objectHeld->setVelocity(getVelocity());
		objectHeld->setZ(1);
	}
	objectHeld = object;
	if(objectHeld.isValid())
	{
		objectHeld->setScale(getScale());
		objectHeld->setZ(2);
		switch(objectHeld->getType())
		{
			case Object::SWORD:
				getPlayer()->setText(0); break;
			case Object::NUKE:
				getPlayer()->setText(1); break;
			case Object::ROCKET_LAUNCHER:
				switch(objectHeld.as<RocketLauncher>()->getRocketType())
				{
					case RocketLauncher::STRAIGHT:
						getPlayer()->setText(2); break;
					case RocketLauncher::CAKE:
						getPlayer()->setText(3); break;
					case RocketLauncher::DRUNK:
						getPlayer()->setText(4); break;
				}
				break;
			case Object::CHAIN_WAND:
				getPlayer()->setText(5); break;
			case Object::SHELL:
				getPlayer()->setText(6); break;
		}
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
		case CHAIN_WAND:
		case SWORD:
			swinging = true;
			heldOrientationOffset = (float)kit::math::PI_OVER_4;
			heldRadiusOffset = getRadius() * .5f;
			break;
		case ROCKET_LAUNCHER:
			objectHeld.as<RocketLauncher>()->fire();
			break;
		case SHELL:
		{
			Ptr<Object> o = objectHeld;
			Vector2f d = Vector2f(std::cos(getOrientation()), std::sin(getOrientation()));
			setObjectHeld(Ptr<Object>());
			o->setHeld(Ptr<Character>());
			o->setPosition(getPosition() + 32.f * d);
			o->setVelocity(400.f * d);
			o.as<Shell>()->setOwned(getPlayer()->getNumber());
			break;
		}
		case NUKE:
			objectHeld.as<Nuke>()->explode(getPlayer()->getNumber());
			break;
	}
}

void Character::setCrazy()
{
	crazy = true;
	crazyStart = kit::app::getTime();
}

void Character::harm(int owned, float amount)
{
	if(health == 0)
	{
		return;
	}
	if(amount > health)
	{
		amount = health;
	}
	if(owned != -1 && player->getNumber() != owned)
	{
		game->players[owned]->addScore((int)amount);
	}
	int r = kit::math::random(0, 3);
	kit::audio::play("sounds/hurt" + std::to_string(r) + ".ogg");
	health -= amount;
	if(health <= 0)
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
	for(int i = 0; i < 4; i++)
	{
		OwnPtr<RobotBody> body;
		body.create(level, filename, i);
		body->setPosition(getPosition() + Vector2f(kit::math::random(-32.f, +32.f), kit::math::random(-32.f, +32.f)));
		body->setVelocity(getVelocity() + Vector2f(kit::math::random(-32.f, +32.f), kit::math::random(-32.f, +32.f)));
		body->setOrientation(kit::math::random(0.f, (float)kit::math::TWO_PI));
		body->setScale(getScale());
		level->addObject(body);
	}
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
	if(health == 0)
	{
		return;
	}
	if(getPosition()[0] < 0 || getPosition()[1] < 0 || getPosition()[0] > level->getSize()[0] * level->getTileSize()[0] || getPosition()[1] > level->getSize()[1] * level->getTileSize()[1])
	{
		harm(getPlayer()->getNumber(), .1f);
	}
	if(getVelocity().norm() > maxSpeed)
	{
		setVelocity(getVelocity().unit() * maxSpeed);
	}
	if(crazy)
	{
		if(kit::app::getTime() < crazyStart + 10.f)
		{
			if(kit::math::random(0.f, .25f) < dt)
			{
				applyImpulse(Vector2f(kit::math::random(-1000.f, +1000.f), kit::math::random(-1000.f, +1000.f)));
			}
		}
		else
		{
			crazy = false;
		}
	}
	if(getScale() != 1.f)
	{
		setScale(.001f + .999f * getScale());
		if(std::abs(getScale() - 1.f) < .01)
		{
			setScale(1.f);
		}
		if(objectHeld.isValid())
		{
			objectHeld->setScale(getScale());
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
		objectHeld->setPosition(getPosition() + (getRadius() * getScale() + heldRadiusOffset) * Vector2f(std::cosf(getOrientation() + heldOrientationOffset), std::sinf(getOrientation() + heldOrientationOffset)));
		objectHeld->setOrientation(getOrientation() + heldOrientationOffset);
	}
}


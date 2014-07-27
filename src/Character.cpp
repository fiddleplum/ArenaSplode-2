#include "Character.h"
#include "RocketLauncher.h"
#include <kit/math_util.h>

Character::Character(Ptr<Level> level, std::string const & characterFilename)
: Object(CHARACTER, level, "art/characters/" + characterFilename, Recti::minSize(0, 0, 64, 64))
{
	setSolid(true);
	setZ(3);

	heldOrientationOffset = 0;
	heldRadiusOffset = 0;

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

void Character::update(float dt)
{
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


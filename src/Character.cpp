#include "Character.h"
#include <kit/math_util.h>

Character::Character(Ptr<kit::scene::Scene> scene, std::string const & characterFilename)
: Object(CHARACTER, scene, "art/characters/" + characterFilename, Recti::minSize(0, 0, 64, 64))
{
	setSolid(true);
	setZ(2);

	heldOrientationOffset = 0;
	heldRadiusOffset = 0;

	swinging = false;
}

void Character::setObjectHeld(Ptr<Object> object)
{
	if(object.isNull() && objectHeld.isValid())
	{
		objectHeld->setVelocity(getVelocity());
	}
	objectHeld = object;
}

void Character::useHeld()
{
	if(!objectHeld.isValid())
	{
		return;
	}
	if(objectHeld->getType() == SWORD)
	{
		swinging = true;
		heldOrientationOffset = (float)kit::math::PI_OVER_4;
		heldRadiusOffset = getRadius() * .5f;
	}
}

void Character::update(float dt)
{
	if(objectHeld.isValid())
	{
		objectHeld->setPosition(getPosition() + (getRadius() + heldRadiusOffset) * Vector2f(std::cosf(getOrientation() + heldOrientationOffset), std::sinf(getOrientation() + heldOrientationOffset)));
		objectHeld->setOrientation(getOrientation() + heldOrientationOffset);
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


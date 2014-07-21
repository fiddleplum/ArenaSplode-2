#include "Character.h"

Character::Character(Ptr<kit::scene::Scene> scene, std::string const & characterFilename)
: Object(CHARACTER, scene, "art/characters/" + characterFilename, Recti::minSize(0, 0, 64, 64))
{
	setSolid(true);
	setZ(2);
	heldSwingOffset = 0;
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
		heldSwingDown = true;
		heldSwingOffset = 3.14159f / 4.f;
		heldRadiusOffset = getRadius() * .5f;
	}
}

void Character::update(float dt)
{
	if(objectHeld.isValid())
	{
		objectHeld->setPosition(getPosition() + (getRadius() + heldRadiusOffset) * Vector2f(std::cosf(getOrientation() + heldSwingOffset), std::sinf(getOrientation() + heldSwingOffset)));
		objectHeld->setOrientation(getOrientation() + heldSwingOffset);
	}
	if(heldSwingDown)
	{
		heldSwingOffset -= 1.f * dt;
		if(heldSwingOffset < -3.14159f / 4.f)
		{
			heldSwingDown = false;
			heldSwingOffset = 0;
			heldRadiusOffset = 0;
		}
	}
}


#include "Character.h"

Character::Character(Ptr<kit::scene::Scene> scene, std::string const & characterFilename)
: Object(CHARACTER, scene, "art/characters/" + characterFilename, Recti::minSize(0, 0, 64, 64))
{
	setSolid(true);
	setZ(2);
}

void Character::setObjectHeld(Ptr<Object> object)
{
	objectHeld = object;
}


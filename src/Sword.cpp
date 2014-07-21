#include "Sword.h"

Sword::Sword(Ptr<kit::scene::Scene> scene)
: Object(SWORD, scene, "art/items.png", Recti::minSize(0, 0, 64, 64))
{
	setSolid(false);
	setFriction(.25f);
}

void Sword::update(float dt)
{

}


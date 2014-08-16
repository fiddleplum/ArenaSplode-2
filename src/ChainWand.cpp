#include "ChainWand.h"
#include "Character.h"
#include "Level.h"
#include "Chain.h"
#include <kit/audio.h>

ChainWand::ChainWand(Ptr<Level> level)
: Object(CHAIN_WAND, level, "art/items.png", Recti::minSize(0, 128, 64, 64))
{
	setSolid(false);
	setFriction(.99f);
}

void ChainWand::onTouch(Ptr<Object> object)
{
	if(object->getType() == Object::CHARACTER && getHeldCharacter().isValid() && getHeldCharacter() != object && getHeldCharacter()->isSwinging())
	{
		if(!character1.isValid())
		{
			character1 = object.as<Character>();
		}
		else if(character1 != object)
		{
			OwnPtr<Chain> chain;
			chain.create(character1, object.as<Character>(), level);
			level->addObject(chain);
		}
	}
}


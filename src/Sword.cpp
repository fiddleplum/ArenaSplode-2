#include "Sword.h"
#include "Character.h"
#include "Level.h"
#include "Player.h"
#include <kit/audio.h>
#include <kit/math_util.h>

Sword::Sword(Ptr<Level> level)
: Object(SWORD, level, "art/items.png", Recti::minSize(0, 0, 64, 64))
{
	setSolid(false);
	setFriction(.99f);
}

void Sword::onTouch(Ptr<Object> object)
{
	if(object->getType() == Object::CHARACTER && getHeldCharacter().isValid() && getHeldCharacter() != object && getHeldCharacter()->isSwinging())
	{
		Vector2f impulse = (this->getPosition() - object->getPosition()).unit() * 400.f;
		object->applyImpulse(-impulse);
		this->applyImpulse(impulse);
		object.as<Character>()->harm(getHeldCharacter()->getPlayer()->getNumber(), 10.f);
	}
	else if(object->getType() == Object::SWORD && object->getHeldCharacter().isValid())
	{
		kit::audio::play("sounds/sword" + std::to_string(kit::math::random(0, 3)) + ".ogg");
		Vector2f impulse = (this->getPosition() - object->getPosition()).unit() * 400.f;
		object->applyImpulse(-impulse);
		this->applyImpulse(impulse);
	}
}

void Sword::onOverTile(Vector2i tilePosition)
{
	Tile & tile = level->getTile(tilePosition);
	if(getHeldCharacter().isValid() && getHeldCharacter()->isSwinging())
	{
		if(tile.type == Tile::Wall)
		{
			kit::audio::play("sounds/sword" + std::to_string(kit::math::random(0, 3)) + ".ogg");
		}
	}
}
#pragma once

#include "Object.h"

class Sword : public Object
{
public:
	Sword(Ptr<Level> level);

	void onTouch(Ptr<Object> object) override;

	void onOverTile(Vector2i tilePosition, Vector2f closest) override;

private:
	float lastPlayedSwordSound;
};


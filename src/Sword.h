#pragma once

#include "Object.h"

class Sword : public Object
{
public:
	Sword(Ptr<Level> level);

	void onTouch(Ptr<Object> object) override;

	void onOverTile(Vector2i tilePosition) override;

private:
};


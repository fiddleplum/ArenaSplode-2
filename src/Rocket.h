#pragma once

#include "Object.h"

class Rocket : public Object
{
public:
	enum { STRAIGHT, CAKE, DRUNK };

	Rocket(int owned, Ptr<Level> level, int type);

	int getRocketType() { return type; }

	void update(float dt) override;

	void onTouch(Ptr<Object> object) override;

	void onOverTile(Vector2i position, Vector2f closest) override;

	void setExplode(float delay);

private:
	void explode();

	int owned;
	int type;
	bool willExplode;
	float explodeCountdown;
	float ttl;
};


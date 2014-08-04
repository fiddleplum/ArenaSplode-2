#pragma once

#include "Object.h"

class Rocket : public Object
{
public:
	enum { STRAIGHT, SHRINKER, DRUNK };

	Rocket(Ptr<Level> level, int type);

	int getRocketType() { return type; }

	void update(float dt) override;

	void onTouch(Ptr<Object> object) override;

	void onOverTile(Vector2i position) override;

	void setExplode(float delay);

private:
	void explode();

	int type;
	bool willExplode;
	float explodeCountdown;
	float ttl;
};


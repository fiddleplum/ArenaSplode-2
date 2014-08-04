#pragma once

#include "Object.h"

class RocketLauncher : public Object
{
public:
	enum { STRAIGHT, SHRINKER, DRUNK, NUM_ROCKET_LAUNCHER_TYPES };
	RocketLauncher(Ptr<Level> level, int type);

	void fire();

private:
	int type;
};


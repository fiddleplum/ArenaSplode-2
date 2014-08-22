#pragma once

#include "Object.h"

class RocketLauncher : public Object
{
public:
	enum
	{
		STRAIGHT, CAKE, DRUNK, NUM_ROCKET_LAUNCHER_TYPES
	};
	RocketLauncher(Ptr<Level> level, int type);

	int getRocketType() const
	{
		return type;
	}

	void fire();

private:
	int type;
};


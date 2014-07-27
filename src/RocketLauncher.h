#pragma once

#include "Object.h"

class RocketLauncher : public Object
{
public:
	RocketLauncher(Ptr<Level> level);

	void fire();

private:
};


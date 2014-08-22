#pragma once

#include "Object.h"

class Nuke : public Object
{
public:
	Nuke(Ptr<Level> level);

	void explode(int owned);

private:
	bool exploding;
};


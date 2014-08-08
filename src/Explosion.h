#pragma once

#include "Object.h"

class Explosion : public Object
{
public:
	Explosion(int owned, Ptr<Level> level);

	void update(float dt) override;

private:
	int owned;
	float time = 0.f;
	float doneExplodingTime = .5f;
};


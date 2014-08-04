#pragma once

#include "Object.h"

class Explosion : public Object
{
public:
	Explosion(Ptr<Level> level);

	void update(float dt) override;

private:
	float time = 0.f;
	float doneExplodingTime = .5f;
};


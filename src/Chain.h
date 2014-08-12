#pragma once

#include "Object.h"

class Chain : public Object
{
public:
	Chain(Ptr<Character> character1, Ptr<Character> character2, Ptr<Level> level);

	void update(float dt) override;

	void preRenderUpdate() override;

private:
	Ptr<Character> character1;
	Ptr<Character> character2;
};


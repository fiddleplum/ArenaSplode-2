#pragma once

#include "Object.h"

class Chain : public Object
{
public:
	Chain(Ptr<Character> character1, Ptr<Character> character2, Ptr<Level> level);

	bool holdsCharacter(Character const * character);

	void onTouch(Ptr<Object> object) override;

	void update(float dt) override;

	void preRenderUpdate() override;

private:
	float startTime;
	Ptr<Character> character1;
	Ptr<Character> character2;
};


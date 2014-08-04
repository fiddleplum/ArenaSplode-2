#pragma once

#include "Object.h"

class Shrinker : public Object
{
public:
	Shrinker(Ptr<Level> level);

	void onTouch(Ptr<Object> object) override;

private:
};


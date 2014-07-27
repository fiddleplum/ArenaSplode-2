#pragma once

#include "Object.h"

class Rocket : public Object
{
public:
	Rocket(Ptr<Level> level);

	void onTouch(Ptr<Object> object) override;

private:
};


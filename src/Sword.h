#pragma once

#include "Object.h"

class Sword : public Object
{
public:
	Sword(Ptr<kit::scene::Scene> scene);

	void update(float dt) override;

private:
};


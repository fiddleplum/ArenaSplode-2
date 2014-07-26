#pragma once

#include "Object.h"

class Sword : public Object
{
public:
	Sword(Ptr<kit::scene::Scene> scene);

	void onTouch(Ptr<Object> object) override;

private:
};


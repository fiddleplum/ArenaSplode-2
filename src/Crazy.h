#pragma once

#include "Object.h"

class Crazy : public Object
{
public:
	Crazy(Ptr<Level> level);

	void onTouch(Ptr<Object> object) override;

private:
};


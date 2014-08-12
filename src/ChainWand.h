#pragma once

#include "Object.h"

class ChainWand : public Object
{
public:
	ChainWand(Ptr<Level> level);

	void onTouch(Ptr<Object> object) override;

private:
	Ptr<Character> character1;
};


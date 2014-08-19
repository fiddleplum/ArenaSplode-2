#pragma once

#include "Object.h"

class Shell : public Object
{
public:
	enum { GREEN };

	Shell(int owned, Ptr<Level> level, int type);

	int getShellType() { return type; }

	void setOwned(int owned);

	void update(float dt) override;

	void onTouch(Ptr<Object> object) override;

private:
	int owned;
	int type;
	float ttl;
};


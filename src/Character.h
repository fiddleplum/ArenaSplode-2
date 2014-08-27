#pragma once

#include "Object.h"

class Player;

class Character : public Object
{
public:
	Character(Player * player, Ptr<Level> level, std::string const & characterFilename);

	Player * getPlayer() const
	{
		return player;
	}

	float getMaxSpeed() const
	{
		return maxSpeed;
	}
	void setMaxSpeed(float _maxSpeed)
	{
		maxSpeed = _maxSpeed;
	}

	Ptr<Object> getObjectHeld() const
	{
		return objectHeld;
	}
	void setObjectHeld(Ptr<Object> object);

	bool isSwinging()
	{
		return swinging;
	}

	void setCrazy();

	void useHeld();

	void harm(int owned, float amount);

	void die();

	void incNumKills();

	void update(float dt) override;
	void preRenderUpdate() override;

private:
	Player * player;
	Ptr<Object> objectHeld;

	float maxSpeed;

	int numKills;
	bool swinging;
	float heldOrientationOffset;
	float heldRadiusOffset;

	float health;
	float harmSoundTime;

	std::string filename;

	bool crazy;
	float crazyStart;
};


#ifndef _J1LOGIC_H_
#define _J1LOGIC_H_

#include "j1Module.h"
#include "Animation.h"
#include"p2Point.h"

#define gravity 0.3

enum player_logic
{
	IDLE,
	RIGHT,
	LEFT,
	JUMP,
	DEAD,
	FALL
};

class j1Logic : public j1Module
{
public:
	j1Logic()
	{
		v.x = 0;
		v.y = 0;
	}

	bool Logic_Update();

	void Logic_OnCollision(Collider* C1, Collider* C2);

private:
	//void SetAnimation();

public:
	fPoint virtualPosition;
	fPoint v;
	iPoint position;

	player_logic state;

	bool Colliding_Ground;
	bool Colliding_Left;
	bool Colliding_Right;
	int CamPosition;

	Collider* collider;
	Collider* collidingfloor;

	float speed;
	float jump_intensity;

	int camPositionX;
	int camPositionY;
};

#endif

#ifndef _J1LOGIC_H_
#define _J1LOGIC_H_

#include "j1Module.h"
#include "Animation.h"
#include"p2Point.h"



enum player_logic
{
	IDLE,
	RIGHT,
	LEFT,
	JUMP,
	DEAD,
	FALL,
	GOD
};

enum camera_logic
{
	START,
	END,
	FORWARD,
	BACKWARD
};
class j1Logic : public j1Module
{
public:
	j1Logic()
	{
		velocity.x = 0;
		velocity.y = 0;
	}

	bool Logic_Update();

	

	void SetAnimation();

private:
	

public:
	fPoint virtualPosition;
	fPoint velocity;
	iPoint position;
	iPoint End_Position;
	Animation* animation = nullptr;
	Animation idle_right;
	Animation idle_left;
	Animation right;
	Animation left;
	Animation jumping_left;
	Animation jumping_right;
	Animation falling_left;
	Animation falling_right;

	player_logic state;

	bool Colliding_Ground;
	bool Colliding_Left;
	bool Colliding_Right;
	int RelCamPositionX;
	int RelCamPositionY;
	Collider* collider ;
	Collider* collidingfloor = nullptr;

	float speed;
	float jump_intensity;
	float gravity;
	iPoint maxVel = { 3,3 };

	int camPositionX;
	int camPositionY;

	bool CameraBack;
	camera_logic cam_logic;
};

#endif

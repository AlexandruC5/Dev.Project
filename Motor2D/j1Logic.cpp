#include "j1Logic.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Scene.h"

bool j1Logic::Logic_Update()
{
	
	

	if (RelCamPositionX > 2 || velocity.x > 0) {
		virtualPosition.x += velocity.x;
		
	}
	
	else if (velocity.x < 0 || RelCamPositionX < 2)
	{

		virtualPosition.x += velocity.x;
		
			
	}
	if (RelCamPositionY > 2 || velocity.y > 0)
		virtualPosition.y += velocity.y;

	else if (RelCamPositionY < 2 || velocity.y < 0)
	{
		virtualPosition.y -= velocity.y;
	}

	Colliding_Left = false;
	Colliding_Right = false;
	return true;
}




void j1Logic::SetAnimation() 
{
	if (velocity.x > 0)
	{
		if (state == JUMP)
		{
			animation = &jumping_right;
		}
		else if (state == FALL) 
		{
			animation = &falling_right;
		}
		else
		{
			animation = &right;
		}
	}
	else if(velocity.x<0)
	{
		if (state == JUMP)
		{
			animation = &jumping_left;
		}
		else if (state == FALL)
		{
			animation = &falling_left;
		}
		else
		{
			animation = &left;
		}
	}
	else
	{
		if (state == IDLE)
		{
			if (animation == &left || animation == &jumping_left || animation == &falling_left)
			{
				animation = &idle_left;
			}
			else if (animation == &right || animation == &jumping_right || animation == &falling_right)
			{
				animation = &idle_right;
			}
		}
		else if (state == JUMP)
		{
			if (animation == &idle_left)
			{
				animation = &jumping_left;
			}
			else if (animation == &idle_right)
			{
				animation = &jumping_right;
			}
		}
	}

}
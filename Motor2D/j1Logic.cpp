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


void j1Logic::OnCollision(Collider* C1, Collider* C2)
{
	//if (C2->type == COLLIDER_FLOOR)
	//{
	//	if ((C2->rect.y - v.y ) > (C1->rect.y + C1->rect.h)) //bottom collision
	//	{
	//		if (Colliding_Ground == true)
	//		{
	//			v.y = 0;
	//			if (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT || v.x == 0)
	//			{
	//				v.x = 0;
	//				state = IDLE;
	//			}
	//			else if (v.x < 0)
	//				state = LEFT;

	//			else if (v.x > 0)
	//				state = RIGHT;

	//			Colliding_Ground = false;
	//			//ground sound FX
	//		}
	//		collidingfloor = C2;

	//	}

	//	else if ((C2->rect.x + C2->rect.w) < (C1->rect.x + 5))
	//	{
	//		if (v.x < 0)
	//		{
	//			v.x = 0;
	//		}
	//		Colliding_Left = true;
	//	}

	//	else if ((C2->rect.x ) > C1->rect.w)
	//	{
	//		if (v.x > 0)
	//		{
	//			v.x = 0;
	//		}
	//		Colliding_Right = true;
	//	}

	//	else if ((C2->rect.y + C2->rect.h) < C1->rect.y) //colliding top
	//	{
	//		if (v.y > 0)
	//			v.y = 0;
	//	}

	//}
	//else if (C2->type == COLLIDER_PLATFORM)
	//{
	//	if ((C2->rect.y - v.y + 1) > (C1->rect.y + C1->rect.h)) // collides when player is above the platform
	//	{
	//		if (Colliding_Ground == false)
	//		{
	//			v.y = 0;

	//			if (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT || v.x == 0)
	//			{
	//				v.x = 0;
	//				state = IDLE;
	//			}
	//			else if (v.x < 0)
	//			{
	//				state = LEFT;
	//			}
	//			else if (v.x > 0)
	//			{
	//				state = RIGHT;
	//			}
	//			Colliding_Ground = true;
	//			//FX colliding ground;
	//		}
	//		collidingfloor = C2;
	//	}
	//}


	
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
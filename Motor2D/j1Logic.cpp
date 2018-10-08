#include "j1Logic.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"


bool j1Logic::Logic_Update()
{
	v.y += (gravity)*((Colliding_Ground)? 0:1);

	//virtualPosition.y -= v.y;

	if (CamPosition > 2 || v.x > 0)
		virtualPosition.x += v.x;

	if (CamPosition > 2 || v.y > 0)
		virtualPosition.y += v.y;
}


void j1Logic::Logic_OnCollision(Collider* C1, Collider* C2)
{
	if (C2->type == COLLIDER_FLOOR)
	{
		if ((C2->rect.y - v.y + 1) > (C1->rect.y + C1->rect.h)) //bottom collision
		{
			if (Colliding_Ground == false)
			{
				v.y = 0;
				if (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT || v.x == 0)
				{
					v.x = 0;
					state = IDLE;
				}
				else if (v.x < 0)
					state = LEFT;

				else if (v.x > 0)
					state = RIGHT;

				Colliding_Ground = true;
				//ground sound FX
			}
			collidingfloor = C2;
			
		}

		else if ((C2->rect.x + C2->rect.w) < (C1->rect.x + 5))
		{
			if (v.x < 0)
			{
				v.x = 0;
			}
			Colliding_Left = true;
		}

		else if ((C2->rect.x + 4) > C1->rect.w)
		{
			if (v.x > 0)
			{
				v.x = 0;
			}
			Colliding_Right = true;
		}

		else if ((C2->rect.y + C2->rect.h) < C1->rect.y) //colliding top
		{
			if (v.y > 0)
				v.y = 0;
		}
		
	}
	else if (C2->type == COLLIDER_PLATFORM)
	{
		if ((C2->rect.y - v.y + 1) > (C1->rect.y + C1->rect.h)) // collides when player is above the platform
		{
			if (Colliding_Ground == false)
			{
				v.y = 0;

				if (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT || v.x == 0)
				{
					v.x = 0;
					state = IDLE;
				}
				else if (v.x < 0)
				{
					state = LEFT;
				}
				else if (v.x > 0)
				{
					state = RIGHT;
				}
				Colliding_Ground = true;
				//FX colliding ground;
			}
		}
	}

	else if (C2->type == COLLIDER_DEAD)
	{
		if (C1->SetPos);

	}
}
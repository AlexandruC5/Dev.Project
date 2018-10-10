#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "j1Logic.h"

#include <stdio.h>
j1Player::j1Player()
{
	name.create("player");
	
	
	character = { 36,8,25,47 };
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	jump_intensity = config.attribute("jump_intensity").as_float();
	speed = config.attribute("speed").as_float();

	return true;
}

bool j1Player::Start()
{
	LOG("Loading Player");
	bool ret = true;
	if (graphics == nullptr)
		graphics = App->tex->Load("textures/PlayerSpriteSheet.png");

	if (collider == nullptr)
		collider = App->collision->AddCollider({ 0,0,25,47 }, COLLIDER_PLAYER, this);
	collidingfloor = nullptr;
	Colliding_Ground = false;
	Colliding_Left = false;
	Colliding_Right = false;

	v.x = 0;
	v.y = 0;

	virtualPosition.x = position.x;
	virtualPosition.y = position.y;

	//audio

	return ret;

}

bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(graphics);
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}

	return true;
}



bool j1Player::Update(float)
{
	Logic_Update();
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		v.x = +speed;

		if (state != JUMP && state != DEAD)
		{
			state = RIGHT;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		if (v.x > 0 && state != JUMP)
		{
			v.x = 0;
			state = IDLE;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		v.x = -speed;
		if (state != JUMP && state != DEAD)
		{
			state = LEFT;
		}
	}

	else if(App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		if (v.x < 0 && state != JUMP)
		{
			v.x = 0;
			state = LEFT;
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		
		v.y = jump_intensity;
		state = JUMP;
	
	}

	collider->SetPos(position.x + colliderMove.x, position.y + colliderMove.y);

	App->player->Colliding_Left = false;
	App->player->Colliding_Right = false;
	

	return true;
}

bool j1Player::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !Colliding_Right && v.x == 0)
	{
		v.x = speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !Colliding_Left && v.x == 0)
	{
		v.x = -speed;
	}

	p2List_item<ImageLayer*>* image = nullptr; // PARALLAX
	for (image = App->map->data.imagelayers.start; image; image = image->next)
	{
		if (image->data->speed > 0) 
		{
			if (v.x > 0)
			{
				image->data->position.x -= image->data->speed;
			}

			else if (v.x < 0)
			{
				image->data->position.x += image->data->speed;
			}
		}
	}
	position.x = virtualPosition.x;
	position.y = virtualPosition.y;

	int windows_scale = App->win->GetScale();
	camPositionX = App->player->position.x + App->render->camera.x / windows_scale ;
	camPositionY = App->player->position.y + App->render->camera.y / windows_scale ;

	return true;
}


/*void j1Player::OnCollision(Collider* C1, Collider* C2)   //t0 reset double jump
{

}*/

bool j1Player::Load(pugi::xml_node& data)
{
	virtualPosition.x = data.attribute("position_x").as_int();
	virtualPosition.y = data.attribute("position_y").as_int();

	return true;
}

bool j1Player::Save(pugi::xml_node& data) const
{
	data.append_attribute("position_x") = position.x;

	data.append_attribute("position_y") = position.y - 5;

	
	return true;
}
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



	//RIGHT SIDE

	//idle 
	idle_right.PushBack({ 1,2,47,55 });
	idle_right.PushBack({ 49,3,47,54 });
	idle_right.PushBack({ 97,3,49,54 });
	idle_right.PushBack({ 147,3,49,54 });
	idle_right.PushBack({ 197,3,48,54 });
	idle_right.PushBack({ 246,3,47,54 });
	idle_right.PushBack({ 294,3,45,54 });
	idle_right.PushBack({ 340,3,44,54 });
	idle_right.PushBack({ 385,4,41,54 });
	idle_right.PushBack({ 428,4,41,54 });
	idle_right.PushBack({ 471,4,43,54 });

	//right
	right.PushBack({ 2,64,45,54 });
	right.PushBack({ 48,64,48,54 });
	right.PushBack({ 97,64,50,54 });
	right.PushBack({ 149,64,50,54 });
	right.PushBack({ 201,63,48,55 });
	right.PushBack({ 251,63,47,55 });
	right.PushBack({ 300,63,48,55 });
	right.PushBack({ 350,64,48,54 });


	//jumping
	jumping_right.PushBack({ 5,176,47,50 });
	jumping_right.PushBack({ 55,177,47,47 });
	jumping_right.PushBack({ 105,171,42,57 });
	jumping_right.PushBack({ 152,171,42,57 });
	jumping_right.PushBack({ 198,177,47,51 });
	jumping_right.PushBack({ 251,181,51,46 });
	jumping_right.PushBack({ 306,180,51,47 });
	jumping_right.PushBack({ 361,176,51,55 });
	jumping_right.PushBack({ 415,169,47,62 });
	jumping_right.PushBack({ 466,170,49,61 });
	jumping_right.PushBack({ 517,177,47,50 });
	jumping_right.PushBack({ 567,177,48,50 });

	//falling
	falling_right.PushBack({ 415,169,47,62 });
	falling_right.PushBack({ 466,170,49,61 });



	//LEFT SIDE

	//idle 
	idle_left.PushBack({ 10,242,43,54 });
	idle_left.PushBack({ 55,242,41,54 });
	idle_left.PushBack({ 98,242,41,54 });
	idle_left.PushBack({ 140,241,44,54 });
	idle_left.PushBack({ 185,241,45,54 });
	idle_left.PushBack({ 231,241,47,54 });
	idle_left.PushBack({ 279,241,48,54 });
	idle_left.PushBack({ 320,241,49,54 });
	idle_left.PushBack({ 378,241,49,54 });
	idle_left.PushBack({ 428,241,47,54 });
	idle_left.PushBack({ 476,240,47,55 });

	//left
	left.PushBack({ 15,303,48,54 });
	left.PushBack({ 65,302,48,55 });
	left.PushBack({ 115,302,47,55 });
	left.PushBack({ 164,302,48,55 });
	left.PushBack({ 214,303,50,54 });
	left.PushBack({ 266,303,50,54 });
	left.PushBack({ 317,303,48,54 });
	left.PushBack({ 366,303,45,54 });


	//jumping
	jumping_left.PushBack({ 572,424,47,50 });
	jumping_left.PushBack({ 522,425,47,47 });
	jumping_left.PushBack({ 477,419,42,57 });
	jumping_left.PushBack({ 430,419,42,57 });
	jumping_left.PushBack({ 379,425,47,51 });
	jumping_left.PushBack({ 322,429,51,46 });
	jumping_left.PushBack({ 267,428,51,47 });
	jumping_left.PushBack({ 212,424,51,55 });
	jumping_left.PushBack({ 162,418,47,61 });
	jumping_left.PushBack({ 109,418,49,61 });
	jumping_left.PushBack({ 60,425,47,50 });
	jumping_left.PushBack({ 9,425,48,50 });
	

	
	character = { 36,8,25,47 };

	//falling
	falling_left.PushBack({ 109,418,49,61 });
	falling_left.PushBack({ 162,418,47,61 });



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
		collider = App->collision->AddCollider({ App->player->position.x ,App->player->position.y ,25,47 }, COLLIDER_PLAYER, this);
	collidingfloor = nullptr;
	Colliding_Ground = false;
	Colliding_Left = false;
	Colliding_Right = false;

	v.x = 0;
	v.y = 0;
	animation = &idle_right;

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

	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame());

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
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "j1Logic.h"
j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

	level* level1 = new level(1, "map1.tmx");
	level* level2 = new level(2, "map2.tmx");

	levels.add(level1);
	levels.add(level2);


	current_level = levels.start;
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(levels.start->data->map_path.GetString(), current_level->data->length);
	App->player->Start();
	//App->player->position(0, 0);
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();

	/*(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 1;*/

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		CleanUp();
		LoadLevel(1);
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (current_level->data->lvl == 2)
		{
			CleanUp();
			LoadLevel(2);
		}

		else {
			App->map->CleanUp();
			LoadLevel(1);
		}
		
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		LoadLevel(2);
	uint win_width, win_height;
	App->win->GetWindowSize(win_width, win_height);


	max_camera_pos = current_level->data->length + (win_width *3) ;
	max_camera_pos *= -1;
	
	min_camera_pos = 0;

		

	/*if (App->player->position.x >= 0 && App->player->position.x < win_width * App->win->GetScale() / 10)
	{
		App->render->camera.x = -15;

	}*/
	if (App->player->RelCamPositionX > (win_width / App->win->GetScale() / 2) && (App->render->virtualCamPosX > max_camera_pos)) {
		App->render->virtualCamPosX -= App->player->speed * 2; //to run at the same speed as the camera

		
	}

	else if (App->player->RelCamPositionX < (win_width * App->win->GetScale() / 10) && App->render->virtualCamPosX < App->player->position.x - win_width)
	{
		App->render->virtualCamPosX += App->player->speed * 2;
		/*
				if (App->player->position.x < win_width * App->win->GetScale() / 10)
				{

					App->render->virtualCamPosX = -App->player->position.x ;
				}*/
		
		
	}

	 
	

	
	


	
	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	// TODO 7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());

	LOG("%i", App->render->camera.x);
	LOG("%i", App->player->position.x);
	LOG("%f", App->render->virtualCamPosX);
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->collision->CleanUp();
	
	
	return true;
}



void j1Scene::LoadLevel(int num)
{
	if (num == 0)
	{
		current_level = current_level->next;
		if (current_level == nullptr)
		{
			current_level = levels.start;
		}
	}
	else
	{
		p2List_item<level*>* lvl = levels.start;
		


		for (int i = 1; i < num; ++i)
		{
			lvl = lvl->next;
			if (lvl == nullptr)
			{
				LOG("Couldn't load level", num);

				break;
			}
		}
		current_level = lvl;
	}

	if (current_level != nullptr)
	{
		App->map->Load(current_level->data->map_path.GetString(), current_level->data->length);
		App->player->collider = nullptr;
		App->player->Start();

	}
}
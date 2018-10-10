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

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 1;
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->map->Load("map2.tmx", current_level->data->length);
	uint win_width, win_height;
	App->win->GetWindowSize(win_width, win_height);

<<<<<<< HEAD
	//max_camera_pos = current_lvl->data->length + win_width;
	//max_camera_pos *= -1;

	/**/
//	App->render->camera.x = -App->player->position.x + (App->player->position.x +(App->win->GetWindowSize.window.w))
	//if (App->player->camPositionX > (win_width / App->win->GetScale() / 2)) //Moving camera in X label
	//{
	//	App->render->virtualCamPosX = App->player->speed * 2;
	//}
	//else if (App->player->camPositionX < (win_width / App->win->GetScale() / 2))
	//{
	//	App->render->virtualCamPosX = App->player->speed * 2;
	//}
	//if (App->player->camPositionY > (win_height / App->win->GetScale() / 2))
	//{
	//	App->render->virtualCamPosY = App->player->speed * 2;
	//}
	//else if (App->render->virtualCamPosY < (win_height / App->win->GetScale() / 2))
	//{
	//	App->render->virtualCamPosY = App->player->speed * 2;
=======
	/*max_camera_pos = current_lvl->data->length + win_width;
	max_camera_pos *= -1;*/

	//if (App->player->camPositionX > (win_width / App->win->GetScale() / 2)) //Moving camera in X label
	//{
	//	App->render->virtualCamPosX += App->player->speed * 2;
	//}
	//else if (App->player->camPositionX < (win_width / App->win->GetScale() / 2))
	//{
	//	App->render->virtualCamPosX += App->player->speed * 2;
	//}
	//if (App->player->camPositionY > (win_height / App->win->GetScale() / 2))
	//{
	//	App->render->virtualCamPosY += App->player->speed * 2;
	//}
	//else if (App->render->virtualCamPosY < (win_height / App->win->GetScale() / 2))
	//{
	//	App->render->virtualCamPosY += App->player->speed * 2;
>>>>>>> 19c7d1269cf5f1f033dc2b5ab6043582c272dc22
	//}
	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	// TODO 7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
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
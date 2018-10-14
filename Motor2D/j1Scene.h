#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Logic.h"
struct SDL_Texture;

struct level
{

	p2SString map_path;
	int lvl;
	int length;
	int height;
	iPoint Start_Point;
	iPoint End_Point;
	level(int num, char* map_path)
	{
		this->map_path.create(map_path);
		lvl = num;
	}
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//if num = 0 will search next level
	void LoadLevel(int num = 0);

public:

	p2List<level*> levels;
	p2List_item<level*>* current_level;

	int max_camera_posX;
	int max_camera_posY;

	camera_logic cameralogic;

	bool stop;
};

#endif // __j1SCENE_H__
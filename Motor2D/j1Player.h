#ifndef  __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "j1Logic.h"
#include "Animation.h"
#include"p2Point.h"

struct SDL_Texture;
struct Collider;

class j1Player : public j1Logic
{
public:
	j1Player();
	j1Player();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* C1, Collider C2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;

public:
};
#endif // !1


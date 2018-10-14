#ifndef  __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "j1Logic.h"
#include "Animation.h"
#include"p2Point.h"

struct SDL_Texture;
struct Collider;
struct SDL_Rect;

class j1Player : public j1Logic
{
public:
	j1Player();
	~j1Player();
	//j1Player()
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float);
	bool PostUpdate();
	int DeathMargin = 1400;
	bool CleanUp();
	void OnCollision(Collider* C1, Collider* C2);
	void SetActions();
	void SetState();
	bool FALLING = false;
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;
	SDL_Rect character;
	SDL_Texture* graphics;

	void GodMode();

	fPoint acceleration;

	fPoint colliderMove;

	bool doublejump;
	bool movingGodModeY;
	uint win_fx;
	uint jump_fx;
	uint death_fx;
	
public:
};
#endif // !1


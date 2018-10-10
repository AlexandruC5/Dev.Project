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
	bool CleanUp();
	//void OnCollision(Collider* C1, Collider* C2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;
	SDL_Rect character;
	SDL_Texture* graphics;

	fPoint colliderMove;

	
public:
};
#endif // !1


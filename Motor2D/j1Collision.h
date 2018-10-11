#ifndef _J1COLLISION_H
#define _J1COLLISION_H

#define MAX_COLLIDERS 400

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "j1Player.h"

enum COLLIDER_TYPE 
{
	COLLIDER_NONE = -1,
	COLLIDER_FLOOR,
	COLLIDER_PLAYER,
	COLLIDER_PLATFORM, //staying under the platform jump through itself and stay above 
	MATRIX_COLLIDER
};


struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;
	bool enable;
	Collider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rect),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
		
	}

	void SetShape(int w, int h)
	{
		rect.w = w;
		rect.h = h;
	}

	void SetType(COLLIDER_TYPE type)
	{
		this->type = type;
	}

	


	bool CheckCollision(const SDL_Rect& r) const;

	bool Update();
};

class j1Collision : public j1Module {
public:
	j1Collision();
	~j1Collision();

	bool PreUpdate();
	bool Update(float);

	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	bool DeleteCollider(Collider* collider);
	void DebugColliders();

private:
	Collider* colliders[MAX_COLLIDERS];
	bool matrix[MATRIX_COLLIDER][MATRIX_COLLIDER];

	bool debug = false;
};
#endif

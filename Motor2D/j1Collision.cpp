#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"


j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		matrix[COLLIDER_FLOOR][COLLIDER_FLOOR] = false;
		matrix[COLLIDER_FLOOR][COLLIDER_PLAYER] = false;
		matrix[COLLIDER_FLOOR][COLLIDER_PLATFORM] = false;
		

		matrix[COLLIDER_PLAYER][COLLIDER_FLOOR] = true;
		matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
		matrix[COLLIDER_PLAYER][COLLIDER_PLATFORM] = true;
		

		matrix[COLLIDER_PLATFORM][COLLIDER_FLOOR] = false;
		matrix[COLLIDER_PLATFORM][COLLIDER_PLAYER] = false;
		matrix[COLLIDER_PLATFORM][COLLIDER_PLATFORM] = false;
		

		
	}
	
}


j1Collision::~j1Collision()  {}

bool j1Collision::PreUpdate()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i) 
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true) {
			delete colliders[i];
			colliders[i] = nullptr;
		}


	}

	return true;
}


bool j1Collision::Update(float)
{
	Collider* C1;
	Collider* C2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] == nullptr) continue; //skip empty colliders

		C1 = colliders[i];
		C1->Update();

		for (uint j = i+1; j < MAX_COLLIDERS; ++j) { // avoid cheking coll already checked

			if (colliders[j] == nullptr) continue;

			C2 = colliders[j];
			C2->Update();

			if (C1->CheckCollision[C2->rect] == true)
			{
				if (matrix[C1->type][C2->type] && C1->callback)
					C1->callback->OnCollision(C1, C2);

				if (matrix[C2->type][C1->type] && C2->callback)
					C2->callback->OnCollision(C2, C1);
			}

			//else if() player.cpp

		}
	}
	DebugColliders();
	return true;
}



void j1Collision::DebugColliders()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == false) return;

	Uint8 opacity = 80;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		switch (colliders[i]->type) {

		case COLLIDER_NONE:	
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, opacity); //white
			break;

		case COLLIDER_FLOOR:
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, opacity); //red
			break;

		case COLLIDER_PLAYER:
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, opacity); //blue
			break;

		case COLLIDER_PLATFORM:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, opacity); //green
			break;

		case COLLIDER_DEAD:
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, opacity); //yellow
			break;
		}
	}
}

bool j1Collision::CleanUp() {

	LOG("cleaning coliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i) 
	{

		if (colliders[i] != nullptr) 
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider( rect, type, callback);
		}
	}
	return ret;
}


bool j1Collision::DeleteCollider(Collider* collider) {
	if (collider != nullptr) 
	{
		for (uint i = 0; i < MAX_COLLIDERS; ++i) //still searching for a lost pointer
		{
			if (colliders[i] == collider)
			{
				collider->to_delete = true;
				break;
			}
		}
	}
	return false;
}
bool Collider::Update()
{
	return true;
}

bool Collider::CheckCollision(const SDL_Rect& r) const {
	return (rect.x < r.x + r.w &&
			rect.x + rect.w > r.x &&
			rect.y < r.y + r.h &&
			rect.h < + rect.y >r.y);
}
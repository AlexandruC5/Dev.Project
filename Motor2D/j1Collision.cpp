#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1Player.h"

j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		colliders[i] == nullptr;
		matrix[COLLIDER_FLOOR][COLLIDER_FLOOR] = false;
		matrix[COLLIDER_FLOOR][COLLIDER_PLAYER] = true;
		matrix[COLLIDER_FLOOR][COLLIDER_PLATFORM] = false;
		

		matrix[COLLIDER_PLAYER][COLLIDER_FLOOR] = true;
		matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
		matrix[COLLIDER_PLAYER][COLLIDER_PLATFORM] = true;
		

		matrix[COLLIDER_PLATFORM][COLLIDER_FLOOR] = false;
		matrix[COLLIDER_PLATFORM][COLLIDER_PLAYER] = true;
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

		for (uint j = i + 1; j < MAX_COLLIDERS; ++j) { // avoid cheking coll already checked

			if (colliders[j] == nullptr) continue;

			C2 = colliders[j];
			C2->Update();

			if (C1->CheckCollision(C2->rect) == true)
			{
				if (matrix[C1->type][C2->type] && C1->callback)
					C1->callback->OnCollision(C1, C2);

				if (matrix[C2->type][C1->type] && C2->callback)
					C2->callback->OnCollision(C2, C1);
			}
			else if ((C1 == App->player->collidingfloor || C2 == App->player->collidingfloor) && ((C1->type == COLLIDER_PLAYER && (C2->type == COLLIDER_FLOOR || C2->type == COLLIDER_PLATFORM)) || (C2->type == COLLIDER_PLAYER && (C1->type == COLLIDER_FLOOR || C1->type == COLLIDER_PLATFORM)))) {
				App->player->Colliding_Ground = false;
				if (App->player->state != JUMP)
					App->player->state = FALL;
			}
		}
	}
	DebugColliders();
	return true;
}



void j1Collision::DebugColliders()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false) return;

	Uint8 opacity = 80;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {

		if (colliders[i] == nullptr) continue;
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

		}
	}
}

bool j1Collision::CleanUp() {
	
	LOG("cleaning coliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i) 
	{
		if (colliders[i] == nullptr) LOG("Colliders did not load");

		if (colliders[i] != nullptr) 
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	
	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider( rect, type, callback);
			break;
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


bool Collider::CheckCollision(const SDL_Rect& r) const {
	return ((r.x < rect.x + rect.w) && (rect.x < r.x + r.w)
		&& (r.y < rect.y + rect.h) && (rect.y < r.y + r.h));
}

bool Collider::Update()
{
	return true;
}
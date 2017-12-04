#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleCollision.h"
struct SDL_Texture;

class ModulePlayer : public Module, public Collider::CListener
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();
	Collider* collider = new Collider({ 0, 0, 5, 5 }, PLAYER);
	virtual void OnCollide() override;
	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation up;
	Animation down;
	iPoint position;
	bool destroyed = false;

private:
	int playerSpeed = 1;
};

#endif
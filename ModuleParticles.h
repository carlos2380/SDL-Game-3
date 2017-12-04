#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleCollision.h"

struct SDL_Texture;
enum TypeParticle { LASER, EXPLOSIONLASER};

struct Particle : Collider::CListener
{
	// TODO 1: Fill in the structure with all the properties you need for each particle
	bool to_delete = false;
	iPoint position = { 0, 0 };
	//SDL_Rect position = { 0, 0, 0, 0 };
	Animation anim;
	int speed = 0;
	int timeToDelete = 0;
	TypeParticle typeParticle;
	// TODO 11: Add an optional collider to each particle
	Collider* collider = new Collider({ 0, 0, 5, 5 }, PARTICLE);
	virtual void OnCollide() override;

	Particle();
	Particle(const Particle& p);
	~Particle();

	void Update();
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status PreUpdate(); // clear dirty particles
	update_status Update(); // draw
	bool CleanUp();

	void AddParticle(const Particle& p, int x, int y); // feel free to expand this call
	void AddExplosion(int x, int y);
private:

	SDL_Texture* graphics = nullptr;
	std::list<Particle*> active;

public:

	// prototype particles go here ...
	unsigned fxparticle;
	Particle* particle;

	unsigned fxexplosion;
	Particle* explosion;
};

#endif // __MODULEPARTICLES_H__
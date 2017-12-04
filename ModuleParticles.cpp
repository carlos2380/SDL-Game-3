#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

#include "SDL/include/SDL_timer.h"
#include "ModulePlayer.h"

ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("rtype/particles.png");


	// TODO 2: Create a prototype for the laser particle
	// audio: rtype/laser.wav
	fxparticle = App->audio->LoadFx("rtype/laser.wav");
	
	// coords: {232, 103, 16, 12}; {249, 103, 16, 12};
	particle = new Particle();
	particle->typeParticle = LASER;
	particle->anim.frames.push_back({ 232, 103, 16, 12 });
	particle->anim.frames.push_back({ 249, 103, 16, 12 });
	particle->speed = 0.1f;

	// TODO 12: Create a new "Explosion" particle 
	// audio: rtype/explosion.wav
	fxexplosion = App->audio->LoadFx("rtype/explosion.wav");
	// coords: {274, 296, 33, 30}; {313, 296, 33, 30}; {346, 296, 33, 30}; {382, 296, 33, 30}; {419, 296, 33, 30}; {457, 296, 33, 30};
	explosion = new Particle();
	explosion->typeParticle = EXPLOSIONLASER;
	explosion->anim.frames.push_back({ 274, 296, 33, 30 });
	explosion->anim.frames.push_back({ 313, 296, 33, 30 });
	explosion->anim.frames.push_back({ 346, 296, 33, 30 });
	explosion->anim.frames.push_back({ 382, 296, 33, 30 });
	explosion->anim.frames.push_back({ 419, 296, 33, 30 });
	explosion->anim.frames.push_back({ 457, 296, 33, 30 });
	explosion->speed = 0.1f;
	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	return true;
}

// PreUpdate to clear up all dirty particles
update_status ModuleParticles::PreUpdate()
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end();)
	{
		if((*it)->typeParticle == EXPLOSIONLASER)
		{
			++(*it)->timeToDelete;
			if ((*it)->timeToDelete == 50)(*it)->to_delete = true;
		}
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

// Update all particle logic and draw them
update_status ModuleParticles::Update()
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
	{
		Particle* p = *it;

		p->Update();
		App->renderer->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));

		// Handle particle fx here ?
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& p, int x, int y)
{
	// TODO 4: Fill in a method to create an instance of a prototype particle
	Particle* newParticle = new Particle(p);
	//SDL_Rect particlePosition = {x, y, 25, 25};
	newParticle->position = {x,y};
	newParticle->speed = 4;
	newParticle->typeParticle = LASER;
	newParticle->collider = App->collision->AddCollider({ x, y, 20, 12 }, PARTICLE);
  	newParticle->collider->listener = newParticle;
	active.push_back(newParticle);
	App->audio->PlayFx(fxparticle);

}

void ModuleParticles::AddExplosion(int x, int y)
{
	Particle* newParticle = new Particle(*App->particles->explosion);
	newParticle->typeParticle = EXPLOSIONLASER;
	newParticle->position = { x,y };
	newParticle->speed = 0;
	active.push_back(newParticle);
	App->audio->PlayFx(fxexplosion);

}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
}

// TODO 3: Fill in a copy constructor
Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), collider(p.collider)
{
	
}

Particle::~Particle()
{
}

void Particle::OnCollide()
{
	App->particles->AddExplosion(this->position.x, this->position.y);
	this->to_delete = true;
	this->collider->to_delete = true;
	
}


void Particle::Update()
{
	// TODO 5: This is the core of the particle logic
	// draw and audio will be managed by ModuleParticle::Update()
	// Note: Set to_delete to true is you want it deleted
	this->position.x += this->speed;
	this->collider->rect.x += this->speed;
	if((this->position.x - App->player->position.x ) > SCREEN_WIDTH)
	{
		this->collider->to_delete = true;
		this->to_delete = true;
	}
}


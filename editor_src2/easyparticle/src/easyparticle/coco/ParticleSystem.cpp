#include "ParticleSystem.h"

using namespace eparticle::coco;

ParticleSystem::ParticleSystem(unsigned int buffer)
{
//	delete[] pStart;
	pLast = pStart = new Particle[buffer];
	pEnd = pStart + buffer;

	emitCounter = 0;

	emission_time = 0;
	count = 0;

	min_life = max_life = 0;

	min_hori = max_hori = 0;
	min_vert = min_vert = 0;

	min_spd = max_spd = 0;

	gravity = 0;

	inertia = 0;

	fadeout_time = 0;

	bounce = true;

	additive_blend = false;

	start_radius = 0;

	orient_to_movement = false;
}

ParticleSystem::~ParticleSystem()
{
	if (pStart != NULL)
		delete[] pStart;
}

void ParticleSystem::draw()
{
	vertex vertices[4];

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);

	glTranslatef(origin.x, origin.y, 0);

	Particle* p = pStart;
	while (p != pLast)
	{
		if (p->life < fadeout_time)
		{
			unsigned char a = 255 * p->life / fadeout_time;
			vertices[0].a = vertices[1].a = vertices[2].a = vertices[3].a = a;
			d2d::Shader::Instance()->color(1, 1, 1, a / 255.0f);
		}
		else
		{
			d2d::Shader::Instance()->color(1, 1, 1, 1);
		}

		float gx = p->position[0] * 0.01f,
			gy = p->position[1] * 0.01f;
		float x = (gx - gy) * 36,
			y = (gx + gy) * 26 + p->position[2] * 0.5f;

		float s = (p->life / p->lifetime) * (p->pc->start_scale - p->pc->end_scale) + p->pc->end_scale;

		d2d::SpriteDraw::time = p->life;
		d2d::SpriteDraw::drawSprite(p->pc->symbol, d2d::Vector(x, y), p->angle, s, s);

		++p;
	}

	glPopAttrib();
	glPopMatrix();
}

void ParticleSystem::update(float dt)
{
	if (active)
	{
		float rate = emission_time / count;
		emitCounter += dt;
		while (emitCounter > rate)
		{
			add();
			emitCounter -= rate;
		}

		life -= dt;
		if (lifetime != -1 && life < 0)
			stop();
	}

	Particle* p = pStart;
	while (p != pLast)
	{
		p->life -= dt;

		if (p->life > 0)
		{
			p->speed[2] -= gravity * dt;
			p->position[0] += p->speed[0] * dt;
			p->position[1] += p->speed[1] * dt;
			p->position[2] += p->speed[2] * dt;

			if (p->position[2] < 0)
			{
				if (bounce)
				{
					p->position[2] *= -0.2f;
					p->speed[0] *= 0.4f;
					p->speed[1] *= 0.4f;
					p->speed[2] *= -0.2f;
				}
				else
				{
					p->speed[2] = 0;
					p->speed[0] = p->speed[1] = p->speed[2] = 0;
				}
			}

			p->rotate -= inertia * dt;
			if (p->rotate < 0) p->rotate = 0;
			if (p->position[2] > 0.1f)
				p->angle += p->rotate * dt;

			p++;
		}
		else
		{
			remove(p);

			if (p >= pLast)
				return;
		}
	}
}

void ParticleSystem::start()
{
	active = true;
}

void ParticleSystem::stop()
{
	active = false;
	life = lifetime;
	emitCounter = 0;
}

void ParticleSystem::reset()
{
	pLast = pStart;
	life = lifetime = emission_time;
	emitCounter = 0;
}

void ParticleSystem::pause()
{
	active = false;
}

void ParticleSystem::add()
{
	if (isFull()) return;

	std::vector<ParticleChild*> choices;
	for (size_t i = 0, n = children.size(); i < n; ++i)
		if (children[i]->symbol)
			choices.push_back(children[i]);
	if (choices.empty()) return;
	const int index = std::min((size_t) (choices.size() * d2d::Random::getNum0To1()), choices.size()-1);
	pLast->pc = choices[index];

	float min, max;

	min = min_life; max = max_life;
	pLast->life = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	pLast->lifetime = pLast->life;

	min = min_hori; max = max_hori;
	pLast->direction[0] = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	pLast->direction[0] *= d2d::TRANS_DEG_TO_RAD;

	min = min_vert; max = max_vert;
	pLast->direction[1] = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	pLast->direction[1] *= d2d::TRANS_DEG_TO_RAD;

	transCoords(start_radius, pLast->direction[0], pLast->direction[1], pLast->position);

	min = min_spd; max = max_spd;
	float speed = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	transCoords(speed, pLast->direction[0], pLast->direction[1], pLast->speed);

	min = pLast->pc->min_rotate; max = pLast->pc->max_rotate;
	pLast->rotate = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;

	pLast->angle = (rand() / (float(RAND_MAX)+1)) * d2d::PI * 2;

	pLast++;
}

void ParticleSystem::remove(Particle * p)
{
	if (!isEmpty())
		*p = *(--pLast);
}

void ParticleSystem::transCoords(float r, float hori, float vert, float result[3])
{
	float dxy = r * cos(vert);
	float dz = r * sin(vert);
	result[0] = dxy * cos(hori);
	result[1] = dxy * sin(hori);
	result[2] = dz;
}
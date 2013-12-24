#include "ParticleSystem.h"
#include "Matrix.h"

using namespace eparticle::coco3d;

ParticleSystem::ParticleSystem(d2d::Image* sprite, unsigned int buffer)
{
	this->sprite = sprite;

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

	fadeout_time = 0;

	bounce = true;

	additive_blend = false;

	start_radius = 0;

	orient_to_movement = false;

	start_scale = end_scale = 0;

	min_rotate = max_rotate = 0;
}

ParticleSystem::~ParticleSystem()
{
	if (pStart != NULL)
		delete[] pStart;
}

void ParticleSystem::draw()
{
	if (!sprite) return;

	vertex vertices[4];
	
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);

	Particle* p = pStart;
	while (p != pLast)
	{
		glPushMatrix();

		memset(vertices, 255, sizeof(vertex)*4);

		const float hw = sprite->width() * 0.5f,
			hh = sprite->height() * 0.5f;

		vertices[0].x = -hw; vertices[0].y = -hh;
		vertices[1].x = -hw; vertices[1].y = hh;
		vertices[2].x = hw; vertices[2].y = hh;
		vertices[3].x = hw; vertices[3].y = -hh;
		vertices[0].z = vertices[1].z = vertices[2].z = vertices[3].z = 0;

		vertices[0].s = 0; vertices[0].t = 0;
		vertices[1].s = 0; vertices[1].t = 1;
		vertices[2].s = 1; vertices[2].t = 1;
		vertices[3].s = 1; vertices[3].t = 0;

		if (p->life < fadeout_time)
		{
			unsigned char a = 255 * p->life / fadeout_time;
			vertices[0].a = vertices[1].a = vertices[2].a = vertices[3].a = a;
			glColor4ub(255, 255, 255, a);
		}
		else
			glColor4ub(255, 255, 255, 255);

		mat4 translate = mat4::Translate(p->position[0], p->position[1], p->position[2]);
		float s = (p->life / p->lifetime) * (end_scale - start_scale) + start_scale;
		mat4 scale = mat4::Scale(s);
		mat4 rotate = mat4::Rotate(p->angle * d2d::TRANS_RAD_TO_DEG);
		mat4 modelview = rotate * scale * translate;
		glMultMatrixf(modelview.Pointer());

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(vertex), (GLvoid*)&vertices[0].x);
		glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), (GLvoid*)&vertices[0].s);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glPopMatrix();
		++p;
	}

	glPopAttrib();
	glPopMatrix();
}

void ParticleSystem::update(float dt)
{
	Particle* p = pStart;

	float rate = emission_time / count;
	emitCounter += dt;
	while (emitCounter > rate)
	{
		add();
		emitCounter -= rate;
	}

	while (p != pLast)
	{
		p->life -= dt;

		if (p->life > 0)
		{
			p->speed[2] -= gravity * dt;
			p->position[0] += p->speed[0] * dt;
			p->position[1] += p->speed[1] * dt;
			p->position[2] += p->speed[2] * dt;

			if (bounce && p->position[2] < 0)
				p->speed[2] = fabs(p->speed[2]);

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

void ParticleSystem::add()
{
	if (isFull()) return;

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

	min = min_rotate; max = max_rotate;
	pLast->rotate = (rand() / (float(RAND_MAX)+1) * (max - min)) + min;

	pLast->angle = 0;

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
#ifndef EPARTICLE_COCO3D_PARTICLE_SYSTEM_H
#define EPARTICLE_COCO3D_PARTICLE_SYSTEM_H

#include <drag2d.h>

namespace eparticle
{
namespace coco3d
{
	struct vertex
	{
		unsigned char r, g, b, a;
		float x, y, z;
		float s, t;
	};

	struct Particle
	{
		float lifetime;
		float life;

		float position[3];
		float direction[2]; // hori angle and vert angle

//			float speed;
		float speed[3];

		float rotate;
		float angle;

		float scale;
	};

	class ParticleSystem
	{
	private:

		Particle *pStart, *pLast, *pEnd;

		d2d::Image* sprite;

//			bool active;

		// Used to determine when a Particle should be emitted.
		float emitCounter;

		//////////////////////////////////////////////////////////////////////////

		float emission_time;
		int count;

		float min_life, max_life;

		float min_hori, max_hori;
		float min_vert, max_vert;

		float min_spd, max_spd;

		float gravity;

		float fadeout_time;

		bool bounce;

		bool additive_blend;

		float start_radius;

		bool orient_to_movement;

		//////////////////////////////////////////////////////////////////////////

		float start_scale, end_scale;

		float min_rotate, max_rotate;

	public:
		ParticleSystem(d2d::Image* sprite, unsigned int buffer);

		virtual ~ParticleSystem();

		virtual void draw();

		void update(float dt);

	public:
		void setImage(d2d::Image* img) { sprite = img; }
		void setCount(int val) { count = val; }
		void setEmissionTime(int val) { emission_time = val * 0.001f; }
		void setLife(int min, int max) { min_life = min * 0.001f; max_life = max * 0.001f; }
		void setHori(int min, int max) { min_hori = min; max_hori = max; }
		void setVert(int min, int max) { min_vert = min; max_vert = max; }
		void setSpeed(int min, int max) { min_spd = min * 0.25f; max_spd = max * 0.25f; }
		void setGravity(int val) { gravity = val * /*0.001f*/0.25f; }
		void setFadeoutTime(int val) { fadeout_time = val * 0.001f; }
		void setBounce(bool open) { bounce = open; }
		void setScale(int min, int max) { start_scale = min * 0.01f; end_scale = max * 0.01f; } 
		void setRotate(int min, int max) { min_rotate = min * d2d::TRANS_DEG_TO_RAD * 60; max_rotate = max * d2d::TRANS_DEG_TO_RAD * 60; }

	protected:
		void add();
		void remove(Particle * p);

		bool isFull() const { return pLast == pEnd; }
		bool isEmpty() const { return pStart == pLast; }

	private:
		void transCoords(float r, float hori, float vert, float result[3]);
	};
}
}

#endif // EPARTICLE_COCO3D_PARTICLE_SYSTEM_H
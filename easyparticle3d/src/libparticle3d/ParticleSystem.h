#ifndef _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE3D_PARTICLE_SYSTEM_H_

#include <drag2d.h>

namespace eparticle3d
{

struct vertex
{
	unsigned char r, g, b, a;
	float x, y;
	float s, t;
};

class ParticleSystem;

struct ParticleChild
{
	d2d::ISymbol* symbol;

	ParticleSystem* bind_ps;

	float start_scale, end_scale;

	float min_rotate, max_rotate;

	ParticleChild() 
		: symbol(NULL), bind_ps(NULL)
	{}
};

class Recorder;

struct Particle
{
	Particle() : pc(NULL), m_bind_ps(NULL) {}

	d2d::Vector pos;

	float lifetime;
	float life;

	float position[3];
	float direction[2]; // hori angle and vert angle

	float speed[3];

	float rotate;
	float angle;

	float scale;

	ParticleChild* pc;

	ParticleSystem* m_bind_ps;
	float m_bind_time;
};

class ParticleSystem : public d2d::Object, public d2d::ICloneable
{
public:
	d2d::Vector origin;

private:
	// todo
	std::string filepath;

	std::vector<ParticleChild*> children;

	Particle *pStart, *pLast, *pEnd;

	bool active;
	float lifetime;
	float life;

	// Used to determine when a Particle should be emitted.
	float emitCounter;

	//////////////////////////////////////////////////////////////////////////

	float emission_time;
	int count;

	float min_life, max_life;

	float min_hori, max_hori;
	float min_vert, max_vert;

	float min_spd, max_spd;
	float min_angular_spd, max_angular_spd;

	float gravity;

	float inertia;

	float fadeout_time;

	bool bounce;

	bool additive_blend;

	float start_radius;

	bool orient_to_movement;

public:
//	ParticleSystem(/*const ParticleSystem& ps*/);
	ParticleSystem(unsigned int buffer);
	ParticleSystem(const ParticleSystem& ps);

	virtual ~ParticleSystem();

	//
	// ICloneable interface
	//
	virtual ParticleSystem* clone() const { return new ParticleSystem(*this); }

	virtual void draw(const d2d::Matrix& mt, Recorder* recorder = NULL);

	void update(float dt);

	void start();
	void stop();
	void reset();
	void pause();

	bool IsEmpty() const;

	void reloadTexture() const;

	void StoreRecordAsAnimFile(const std::string& filepath) const;

public:
	void setCount(int val) { count = val; }
	void setEmissionTime(int val) { 
		emission_time = val * 0.001f; 
		lifetime = emission_time;
	}
	void setLife(int min, int max) { min_life = min * 0.001f; max_life = max * 0.001f; }
	void setHori(int min, int max) { min_hori = min; max_hori = max; }
	void setVert(int min, int max) { min_vert = min; max_vert = max; }
	void setSpeed(int min, int max) { min_spd = min * 0.25f; max_spd = max * 0.25f; }
	void setAngularSpeed(int min, int max) { min_angular_spd = min; max_angular_spd = max; }
	
	void setGravity(int val) { gravity = val * /*0.001f*//*0.25f*/0.3f; }
	void setInertia(int val) { inertia = val * d2d::TRANS_DEG_TO_RAD; }
	void setFadeoutTime(int val) { fadeout_time = val * 0.001f; }
	void setBounce(bool open) { bounce = open; }
	void setAdditiveBlend(bool open) { additive_blend = open; }
	void setStartRadius(int val) { start_radius = val; }
	void SetOrientToMovement(bool open) { orient_to_movement = open; }

	void addChild(ParticleChild* child) { 
		children.push_back(child); 
	}
	void delChild() { 
		if (!children.empty()) 
		{
			delete children[children.size() - 1];
			children.pop_back(); 
		}
	}

protected:
	void add();
	void remove(Particle * p);

	bool isFull() const { return pLast == pEnd; }
	bool isEmpty() const { return pStart == pLast; }

private:
	void transCoords(float r, float hori, float vert, float result[3]);

	static d2d::Vector TransCoords3To2(float position[3]);

private:
	Recorder* m_recorder;

	friend class FileIO;

}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
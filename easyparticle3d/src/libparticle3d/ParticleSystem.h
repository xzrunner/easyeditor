#ifndef _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE3D_PARTICLE_SYSTEM_H_

#include <easyscale9.h>

#include <drag2d.h>
#include <easy3d.h>

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

	ParticleChild(d2d::ISymbol* symbol) 
		: symbol(symbol), bind_ps(NULL)
	{}
};

class AnimRecorder;
class InvertRecord;

struct Particle
{
	Particle() : pc(NULL), m_bind_ps(NULL) {}

	d2d::Vector pos;

	float lifetime;
	float life;

	float position[3];
	float direction[2]; // hori angle and vert angle

	float speed[3];

	float dis_direction[2];
	float dis_speed;
	float dis_region;
	float dis_curr_len;

	float linear_acc;

	float rotate;
	float angle;

	float scale;

	ParticleChild* pc;

	ParticleSystem* m_bind_ps;
	float m_bind_time;
};

class ParticleSystem : public d2d::Object, public d2d::ICloneable, public d2d::UICallback
{
public:
//	ParticleSystem(/*const ParticleSystem& ps*/);
	ParticleSystem(unsigned int buffer);
	ParticleSystem(const ParticleSystem& ps);

	virtual ~ParticleSystem();

	//
	// ICloneable interface
	//
	virtual ParticleSystem* Clone() const { return new ParticleSystem(*this); }

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const d2d::UICallback::Data& data);
	virtual void GetValue(int key, d2d::UICallback::Data& data);

	virtual void draw(const d2d::Matrix& mt, AnimRecorder* recorder = NULL);

	void update(float dt);

	void SetDirection(float x, float y, float z);
	void SetDirection(const Quaternion& dir);

	void start();
	void stop();
	void reset();
	void pause();
	void SetLoop(bool loop);

	void Clear();

	bool IsEmpty() const;

	void ReloadTexture() const;

	void StoreAnimRecord(const std::string& filepath) const;
	void StoreInvertRecord(const std::string& filepath) const;

public:
	void setHori(int min, int max) { min_hori = min; max_hori = max; }
	void setVert(int min, int max) { min_vert = min; max_vert = max; }
	
	void setInertia(int val) { inertia = val * d2d::TRANS_DEG_TO_RAD; }
	void setBounce(bool open) { bounce = open; }
	void setAdditiveBlend(bool open) { additive_blend = open; }
	void SetOrientToMovement(bool open) { orient_to_movement = open; }
	void SetRadius3D(bool is3d) { is_start_radius_3d = is3d; }

	void addChild(ParticleChild* child) { 
		children.push_back(child); 
	}
	void delChild(int idx) { 
		if (idx >= 0 && idx < children.size()) {
			delete children[idx];
			children.erase(children.begin() + idx);
		}
	}
	void delAllChild() { 
		for (int i = 0, n = children.size(); i < n; ++i) {
			delete children[i];
		}
		children.clear();
	}

protected:
	void add();
	void remove(Particle * p);

	bool isFull() const { return pLast == pEnd; }
	bool isEmpty() const { return pStart == pLast; }

private:
	void TransCoords3D(float r, float hori, float vert, float result[3]);
	void TransCoords2D(float r, float hori, float result[3]);

private:
	AnimRecorder* m_anim_recorder;
	InvertRecord* m_inv_record;

public:
	d2d::Vector origin;

private:
	// todo
	std::string filepath;

	std::vector<ParticleChild*> children;

	Particle *pStart, *pLast, *pEnd;

	bool active;
	bool m_loop;
	float lifetime;
	float life;

	// Used to determine when a Particle should be emitted.
	float emitCounter;

	//////////////////////////////////////////////////////////////////////////

	float emission_time;
	int count;

	mat4 direction;	

	float min_life, max_life;

	float min_hori, max_hori;
	float min_vert, max_vert;

	float min_spd, max_spd;
	float min_angular_spd, max_angular_spd;

	float min_dis_region, max_dis_region;
	float min_dis_spd, max_dis_spd;

	float gravity;

	float min_linear_acc, max_linear_acc;

	float inertia;

	float fadeout_time;

	bool bounce;

	bool additive_blend;

	float start_radius;
	bool is_start_radius_3d;

	bool orient_to_movement;

	friend class FileIO;

}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
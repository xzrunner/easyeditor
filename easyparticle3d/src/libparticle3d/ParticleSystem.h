#ifndef _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE3D_PARTICLE_SYSTEM_H_

#include <drag2d.h>
#include <easy3d.h>

struct particle_system_3d;
struct ps_cfg_3d;
struct particle_symbol;

namespace eparticle3d
{

class AnimRecorder;
class InvertRecord;

class ParticleSystem : public d2d::Object, public d2d::ICloneable, public d2d::UICallback
{
public:
	ParticleSystem(unsigned int buffer, ps_cfg_3d* cfg);
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

	virtual void Draw(const d2d::Matrix& mt, AnimRecorder* recorder = NULL) const;

	void Update(float dt);

	void SetDirection(float x, float y, float z);
	void SetDirection(const Quaternion& dir);

	void Start();
	void Stop();
	void Reset();
	void Pause();
	void SetLoop(bool loop);

	void Clear();

	bool IsEmpty() const;

	void ReloadTexture() const;

	void StoreAnimRecord(const std::string& filepath) const;
	void StoreInvertRecord(const std::string& filepath) const;

	void SetHori(int min, int max);
	void SetVert(int min, int max);
	
	void SetInertia(int val);
	void SetBounce(bool open);
	void SetAdditiveBlend(bool open);
	void SetOrientToMovement(bool open);
	void SetRadius3D(bool is3d);

	particle_symbol* AddSymbol(d2d::ISymbol* symbol);
	void DelSymbol(int idx);
	void DelAllSymbol();

private:
	void Draw(particle_system_3d* ps, const d2d::Matrix& mt, AnimRecorder* recorder = NULL) const;

private:
	AnimRecorder* m_anim_recorder;
	InvertRecord* m_inv_record;

public:
	d2d::Vector m_origin;

private:
	particle_system_3d* m_ps;

}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
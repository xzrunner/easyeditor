#ifndef _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE3D_PARTICLE_SYSTEM_H_

#include <drag2d.h>
#include <easy3d.h>

struct p3d_particle_system;
struct p3d_ps_config;
struct p3d_symbol;
struct p3d_particle;

namespace d3d { class Quaternion; }

namespace eparticle3d
{

class AnimRecorder;
class InvertRecord;

class ParticleSystem : public d2d::Object, public d2d::ICloneable, public d2d::UICallback
{
public:
	ParticleSystem(unsigned int buffer, p3d_ps_config* cfg);
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

	void RemoveFromInvertRecord(p3d_particle* p);

	void SetHori(int min, int max);
	void SetVert(int min, int max);
	
	void SetBounce(bool open);
	void SetOrientToMovement(bool open);
	void SetRadius3D(bool is3d);

 	void SetPosition(const d2d::Vector& pos) { m_pos = pos; }
	const d2d::Vector& GetPosition() const { return m_pos; }

	p3d_symbol* AddSymbol(d2d::ISymbol* symbol);
	void DelSymbol(int idx);
	void DelAllSymbol();

	const p3d_ps_config* GetConfig() const;

	int GetPSCapacity() const;

private:
	void Draw(p3d_particle_system* ps, const d2d::Matrix& mt, AnimRecorder* recorder = NULL) const;

	void SetPSCapacity(int cap);

private:
	AnimRecorder* m_anim_recorder;
	InvertRecord* m_inv_record;

private:
	d2d::Vector m_pos;

	p3d_particle_system* m_ps;

}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
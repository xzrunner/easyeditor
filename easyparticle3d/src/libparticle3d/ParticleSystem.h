#ifndef _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE3D_PARTICLE_SYSTEM_H_

#include <drag2d.h>
#include <easy3d.h>

struct p3d_emitter;
struct p3d_emitter_cfg;
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
	ParticleSystem(p3d_emitter_cfg* cfg, bool record);
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

	void SetEmitterMat(const d2d::Matrix& mt) const;

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

	void SetGround(int ground);

	void SetOrientToMovement(bool open);

	p3d_symbol* AddSymbol(d2d::ISymbol* symbol);
	void DelSymbol(int idx);
	void DelAllSymbol();
	p3d_symbol* GetSymbol(int idx);

	const p3d_emitter_cfg* GetConfig() const;
	const p3d_emitter* GetEmitter() const { return m_et; }

private:
	AnimRecorder* m_anim_recorder;
	InvertRecord* m_inv_record;

private:
	p3d_emitter* m_et;

}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
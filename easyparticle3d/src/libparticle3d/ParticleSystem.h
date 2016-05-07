#ifndef _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE3D_PARTICLE_SYSTEM_H_

#include "PS.h"

#include <ee/Object.h>
#include <ee/Cloneable.h>
#include <ee/UICallback.h>
#include <SM_Quaternion.h>

struct p3d_emitter;
struct p3d_emitter_cfg;
struct p3d_symbol;
struct p3d_particle;

namespace ee { class Symbol; }

namespace eparticle3d
{

class AnimRecorder;
class InvertRecord;

class ParticleSystem : public ee::Object, public ee::Cloneable, public ee::UICallback
{
public:
	ParticleSystem(p3d_emitter_cfg* cfg, bool record);
	ParticleSystem(const ParticleSystem& ps);

	virtual ~ParticleSystem();

	//
	// Cloneable interface
	//
	virtual ParticleSystem* Clone() const { return new ParticleSystem(*this); }

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	virtual void Draw(const ee::Matrix& mt, AnimRecorder* recorder = NULL) const;

	bool Update(const ee::Matrix& mat);
	
	void SetStaticMode(bool is_static);

	void SetDirection(float x, float y, float z);
	void SetDirection(const sm::Quaternion& dir);

	void Start();
	void Stop();
	void Reset();
	void Pause();
	void SetLoop(bool loop);
	void SetLocalModeDraw(bool local);
	bool IsLocalModeDraw() const;

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

	p3d_symbol* AddSymbol(ee::Symbol* symbol);
	void DelSymbol(int idx);
	void DelAllSymbol();
	p3d_symbol* GetSymbol(int idx);

	const p3d_emitter_cfg* GetConfig() const;
	const p3d_emitter* GetEmitter() const;

private:
	void Init(const p3d_emitter_cfg* cfg);

public:
	std::string name;

private:
	mutable RenderParams m_rp;

	AnimRecorder* m_anim_recorder;
	InvertRecord* m_inv_record;

private:
	p3d_sprite* m_spr;

}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
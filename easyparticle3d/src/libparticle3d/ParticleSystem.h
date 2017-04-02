#ifndef _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE3D_PARTICLE_SYSTEM_H_

#include <ee/UICallback.h>

#include <SM_Quaternion.h>
#include <CU_RefCountObj.h>
#include <SM_Matrix.h>
#include <sprite2/Particle3d.h>
#include <sprite2/P3dRenderParams.h>

#include <string>

struct p3d_symbol;
struct p3d_particle;

namespace s2 { class Symbol; class Particle3dEmitter; class P3dEmitterCfg; }

namespace eparticle3d
{

class AnimRecorder;
class InvertRecord;

class ParticleSystem : public cu::RefCountObj, public ee::UICallback
{
public:
	ParticleSystem(const s2::P3dEmitterCfg* cfg, bool record);
	ParticleSystem(const ParticleSystem& ps);

	virtual ~ParticleSystem();

	/**
	 *  @interface
	 *    UICallback
	 */
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	virtual void Draw(AnimRecorder* recorder = NULL) const;

	bool Update(const sm::mat4& mat);

	void SetStaticMode(bool is_static);

	void SetDirection(float x, float y, float z);
	void SetDirection(const sm::Quaternion& dir);

	void Start();
	void Stop();

	void Reset();
	void Pause();

	void SetLoop(bool loop);
	bool IsLoop() const;

	void SetLocal(bool local);
	bool IsLocal() const;

	void Clear();

	bool IsEmpty() const;

	void StoreAnimRecord(const std::string& filepath) const;
	void StoreInvertRecord(const std::string& filepath) const;

	void RemoveFromInvertRecord(p3d_particle* p);

	void SetHori(int min, int max);
	void SetVert(int min, int max);

	void SetGround(int ground);

	void SetOrientToMovement(bool open);

	void SetBlend(int blend);

	p3d_symbol* AddSymbol(s2::Symbol* sym);
	void DelSymbol(int idx);
	void DelAllSymbol();
	p3d_symbol* GetSymbol(int idx);

	const s2::P3dEmitterCfg* GetConfig() const;
// 	const p3d_emitter* GetEmitter() const;

private:
	void InitEmitter(const s2::P3dEmitterCfg* cfg);

public:
	std::string name;

private:
	AnimRecorder* m_anim_recorder;
	InvertRecord* m_inv_record;

private:
	s2::Particle3dEmitter* m_et;

}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_PARTICLE_SYSTEM_H_
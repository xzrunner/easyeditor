#ifndef _EASYPARTICLE2D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE2D_PARTICLE_SYSTEM_H_

#include <ee/UICallback.h>

#include <SM_Matrix.h>
#include <CU_RefCountObj.h>
#include <sprite2/pre_defined.h>
#include S2_MAT_HEADER

struct p2d_emitter_cfg;
struct p2d_emitter;
struct p2d_symbol;

namespace s2 { class Symbol; }

namespace eparticle2d
{

class ParticleSystem : public cu::RefCountObj, public ee::UICallback
{
public:
	ParticleSystem(p2d_emitter_cfg* cfg);
	ParticleSystem(const ParticleSystem& ps);

	virtual ~ParticleSystem();

	/**
	 *  @interface
	 *    UICallback
	 */
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	virtual void Draw(const S2_MAT& mt) const;

	bool Update(const S2_MAT& mat);

	void Start();
	void Stop();
	void Reset();
	void Pause();
	void SetLoop(bool loop);
	void SetLocalModeDraw(bool local);

	void SetMode(int mode);

	void SetRotationIsDir(bool is_dir);

	void Clear();

	bool IsEmpty() const;

	void ReloadTexture() const;

	p2d_symbol* AddSymbol(s2::Symbol* sym);
	void DelSymbol(int idx);
	void DelAllSymbol();

	const p2d_emitter_cfg* GetConfig() const;
	const p2d_emitter* GetEmitter() const { return m_et; }

private:
	sm::vec2 m_pos;

	p2d_emitter* m_et;

}; // ParticleSystem

}

#endif // _EASYPARTICLE2D_PARTICLE_SYSTEM_H_
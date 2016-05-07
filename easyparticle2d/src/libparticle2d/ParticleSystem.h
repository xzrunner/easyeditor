#ifndef _EASYPARTICLE2D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE2D_PARTICLE_SYSTEM_H_

#include <ee/Object.h>
#include <ee/UICallback.h>
#include <ee/Vector.h>

#include <SM_Matrix.h>

struct p2d_emitter_cfg;
struct p2d_emitter;
struct p2d_symbol;

namespace ee { class Symbol; }

namespace eparticle2d
{

class ParticleSystem : public ee::Object, public ee::UICallback
{
public:
	ParticleSystem(p2d_emitter_cfg* cfg);
	ParticleSystem(const ParticleSystem& ps);

	virtual ~ParticleSystem();

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	virtual void Draw(const sm::mat4& mt) const;

	bool Update(const sm::mat4& mat);

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

	p2d_symbol* AddSymbol(ee::Symbol* symbol);
	void DelSymbol(int idx);
	void DelAllSymbol();

	const p2d_emitter_cfg* GetConfig() const;
	const p2d_emitter* GetEmitter() const { return m_et; }

private:
	ee::Vector m_pos;

	p2d_emitter* m_et;

}; // ParticleSystem

}

#endif // _EASYPARTICLE2D_PARTICLE_SYSTEM_H_
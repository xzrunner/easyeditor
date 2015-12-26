#ifndef _EASYPARTICLE2D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE2D_PARTICLE_SYSTEM_H_

#include <drag2d.h>

struct p2d_emitter_cfg;
struct p2d_emitter;
struct p2d_symbol;

namespace eparticle2d
{

class ParticleSystem : public d2d::Object, public d2d::UICallback
{
public:
	ParticleSystem(p2d_emitter_cfg* cfg);
	ParticleSystem(const ParticleSystem& ps);

	virtual ~ParticleSystem();

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const d2d::UICallback::Data& data);
	virtual void GetValue(int key, d2d::UICallback::Data& data);

	virtual void Draw(const d2d::Matrix& mt) const;

	void Update(float dt);

	void Start();
	void Stop();
	void Reset();
	void Pause();
	void SetLoop(bool loop);

	void SetMode(int mode);

	void SetRotationIsDir(bool is_dir);

	void Clear();

	bool IsEmpty() const;

	void ReloadTexture() const;

	p2d_symbol* AddSymbol(d2d::ISymbol* symbol);
	void DelSymbol(int idx);
	void DelAllSymbol();

	const p2d_emitter_cfg* GetConfig() const;

private:
	d2d::Vector m_pos;

	p2d_emitter* m_et;

}; // ParticleSystem

}

#endif // _EASYPARTICLE2D_PARTICLE_SYSTEM_H_
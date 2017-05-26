#ifndef _EASYTRAIL_MOTION_TRAIL_H_
#define _EASYTRAIL_MOTION_TRAIL_H_

#include <ee/UICallback.h>

#include <SM_Matrix.h>
#include <CU_RefCountObj.h>
#include <sprite2/Trail.h>

#include <string>

namespace s2 { class Symbol; class TrailEmitterCfg; class TrailEmitter; }

struct t2d_symbol;
struct t2d_emitter_cfg;

namespace etrail
{

class MotionTrail : public cu::RefCountObj, public ee::UICallback
{
public:
	MotionTrail(s2::TrailEmitterCfg* cfg);
	virtual ~MotionTrail();

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	void Draw(const S2_MAT& mt) const;
	bool Update(const sm::vec2& pos);

	void Start();
	void Stop();

	void Clear();

	t2d_symbol* AddSymbol(s2::Symbol* sym);
	void DelSymbol(int idx);
	void DelAllSymbol();
	t2d_symbol* GetSymbol(int idx);

	const s2::TrailEmitterCfg* GetConfig();

private:
	void Init(const s2::TrailEmitterCfg* cfg);

public:
	std::string name;

private:
	mutable s2::TrailRenderParams m_rp;

	s2::TrailEmitter* m_et;

}; // MotionTrail

}

#endif // _EASYTRAIL_MOTION_TRAIL_H_
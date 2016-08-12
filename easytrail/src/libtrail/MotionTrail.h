#ifndef _EASYTRAIL_MOTION_TRAIL_H_
#define _EASYTRAIL_MOTION_TRAIL_H_

#include "TrailNode.h"

#include <ee/UICallback.h>

#include <SM_Matrix.h>

struct t2d_emitter_cfg;
struct t2d_emitter;
struct t2d_symbol;

namespace ee { class Symbol; }

namespace etrail
{

class MotionTrail : public ee::Object, public ee::UICallback
{
public:
	MotionTrail(t2d_emitter_cfg* cfg);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	void Draw(const sm::mat4& mt) const;
	bool Update(const sm::vec2& pos);

	void Start();
	void Stop();

	void Clear();

	t2d_symbol* AddSymbol(ee::Symbol* symbol);
	void DelSymbol(int idx);
	void DelAllSymbol();
	t2d_symbol* GetSymbol(int idx);

	t2d_emitter_cfg* GetConfig();

private:
	void Init(const t2d_emitter_cfg* cfg);

public:
	std::string name;

private:
	mutable RenderParams m_rp;

	t2d_emitter* m_spr;

}; // MotionTrail

}

#endif // _EASYTRAIL_MOTION_TRAIL_H_
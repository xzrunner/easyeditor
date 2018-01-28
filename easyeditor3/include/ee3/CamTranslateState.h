#pragma once

#include <ee0/EditOpState.h>

#include <SM_Vector.h>

namespace ee { class StageCanvas; }
namespace n3 { class Camera; }

namespace ee3
{

class CamTranslateState : public ee0::EditOpState
{
public:
	CamTranslateState(n3::Camera& cam);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	n3::Camera& m_cam;

	sm::ivec2 m_last_pos;

}; // CamTranslateState

}
#pragma once

#include <ee0/EditOpState.h>

namespace ee { class StageCanvas; }
namespace n3 { class Camera; }

namespace ee3
{

class CamTranslateState : public ee0::EditOpState
{
public:
	CamTranslateState(n3::Camera& cam, const sm::vec2& pos);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseDrag(const sm::vec2& pos) override;

private:
	n3::Camera& m_cam;

	sm::vec2 m_last_pos;

}; // CamTranslateState

}
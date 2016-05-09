#ifndef _EASYCOMPLEX_SPHERE_ROTATE_STATE_H_
#define _EASYCOMPLEX_SPHERE_ROTATE_STATE_H_

#include <ee/ArrangeSpriteState.h>
#include <SM_Quaternion.h>
#include <SM_Vector.h>

namespace ee { class EditPanelImpl; }

namespace ecomplex
{

class SphereRotateState : public ee::ArrangeSpriteState
{
public:
	SphereRotateState(ee::EditPanelImpl* stage, const sm::vec2& first_pos,
		sm::Quaternion& dir);

	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual bool OnMouseDrag(const sm::vec2& pos);

private:
	sm::vec3 MapToSphere(int x, int y);

private:
	ee::EditPanelImpl* m_stage;

	sm::Quaternion& m_dir;

	sm::vec2 m_last_pos;

}; // SphereRotateState

}

#endif // _EASYCOMPLEX_SPHERE_ROTATE_STATE_H_
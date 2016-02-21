#ifndef _EASYCOMPLEX_SPHERE_ROTATE_STATE_H_
#define _EASYCOMPLEX_SPHERE_ROTATE_STATE_H_

#include <ee/ArrangeSpriteState.h>
#include <ee/Vector3D.h>
#include <ee/Quaternion.h>

namespace ee { class EditPanelImpl; }

namespace ecomplex
{

class SphereRotateState : public ee::ArrangeSpriteState
{
public:
	SphereRotateState(ee::EditPanelImpl* stage, const ee::Vector& first_pos,
		ee::Quaternion& dir);

	virtual void OnMouseRelease(const ee::Vector& pos);
	virtual bool OnMouseDrag(const ee::Vector& pos);

private:
	ee::vec3 MapToSphere(int x, int y);

private:
	ee::EditPanelImpl* m_stage;

	ee::Quaternion& m_dir;

	ee::vec2 m_last_pos;

}; // SphereRotateState

}

#endif // _EASYCOMPLEX_SPHERE_ROTATE_STATE_H_
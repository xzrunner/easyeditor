#ifndef _DRAG2D_SPHERE_ROTATE_STATE_H_
#define _DRAG2D_SPHERE_ROTATE_STATE_H_


#include <easy3d.h>

namespace ecomplex
{

class SphereRotateState : public ee::ArrangeSpriteState
{
public:
	SphereRotateState(ee::EditPanelImpl* stage, const ee::Vector& first_pos,
		Quaternion& dir);

	virtual void OnMouseRelease(const ee::Vector& pos);
	virtual bool OnMouseDrag(const ee::Vector& pos);

private:
	vec3 MapToSphere(int x, int y);

private:
	ee::EditPanelImpl* m_stage;

	Quaternion& m_dir;

	vec2 m_last_pos;

}; // SphereRotateState

}

#endif // _DRAG2D_SPHERE_ROTATE_STATE_H_
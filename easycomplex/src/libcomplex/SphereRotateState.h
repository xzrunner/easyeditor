#ifndef _DRAG2D_SPHERE_ROTATE_STATE_H_
#define _DRAG2D_SPHERE_ROTATE_STATE_H_

#include <drag2d.h>
#include <easy3d.h>

namespace ecomplex
{

class SphereRotateState : public d2d::IArrangeSpriteState
{
public:
	SphereRotateState(d2d::EditPanelImpl* stage, const d2d::Vector& first_pos,
		Quaternion& dir);

	virtual void OnMouseRelease(const d2d::Vector& pos);
	virtual bool OnMouseDrag(const d2d::Vector& pos);

private:
	vec3 MapToSphere(int x, int y);

private:
	d2d::EditPanelImpl* m_stage;

	Quaternion& m_dir;

	vec2 m_last_pos;

}; // SphereRotateState

}

#endif // _DRAG2D_SPHERE_ROTATE_STATE_H_
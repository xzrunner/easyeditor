#ifndef _DRAG2D_SPHERE_ROTATE_STATE_H_
#define _DRAG2D_SPHERE_ROTATE_STATE_H_

#include <drag2d.h>
#include <easy3d.h>

namespace ecomplex
{

class SphereRotateState : public d2d::IArrangeSpriteState
{
public:
	SphereRotateState(d2d::EditPanel* stage, const d2d::Vector& first_pos,
		Quaternion& dir);

	virtual d2d::AbstractAtomicOP* OnMouseRelease(const d2d::Vector& pos);
	virtual bool OnMouseDrag(const d2d::Vector& pos);

private:
	vec3 MapToSphere(int x, int y);

private:
	d2d::EditPanel* m_stage;

	Quaternion& m_dir;

	vec2 m_last_pos;

}; // SphereRotateState

}

#endif // _DRAG2D_SPHERE_ROTATE_STATE_H_
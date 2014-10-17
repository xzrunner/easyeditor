#ifndef _LIBSKETCH_ROTATE_OP_H_
#define _LIBSKETCH_ROTATE_OP_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libsketch
{

class RotateOP : public d2d::AbstractEditOP
{
public:
	RotateOP(d2d::EditPanel* stage);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	void SetQuaternion(const Quaternion& ori) { m_ori = ori; }

private:
	vec3 MapToSphere(ivec2 touchpoint) const;

private:
	Quaternion m_ori, m_pre_ori;
	ivec2 m_first_pos;

}; // RotateOP

}

#endif // _LIBSKETCH_ROTATE_OP_H_
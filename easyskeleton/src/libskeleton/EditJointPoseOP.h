#ifndef _LIBSKELETON_EDIT_JOINT_POSE_OP_H_
#define _LIBSKELETON_EDIT_JOINT_POSE_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Matrix.h>

namespace s2 { class Skeleton; class Joint; }

namespace libskeleton
{

class StagePanel;

class EditJointPoseOP : public ee::ZoomViewOP
{
public:
	EditJointPoseOP(StagePanel* stage);
	virtual ~EditJointPoseOP();

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	const s2::Skeleton* m_skeleton;

	s2::Joint* m_selected;

	sm::mat4 m_trans, m_inv_trans;

}; // EditJointPoseOP

}

#endif // _LIBSKELETON_EDIT_JOINT_POSE_OP_H_
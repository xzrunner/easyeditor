#ifndef _LIBSKELETON_EDIT_JOINT_POSE_OP_H_
#define _LIBSKELETON_EDIT_JOINT_POSE_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Vector.h>

namespace ee { class ArrangeSpriteState; }
namespace s2 { class Skeleton; }

namespace libskeleton
{

class StagePanel;
class Sprite;
class Joint;

class EditJointPoseOP : public ee::ZoomViewOP
{
public:
	EditJointPoseOP(StagePanel* stage);
	virtual ~EditJointPoseOP();

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	bool Select(const sm::vec2& pos);

	void ChangeOPState(ee::ArrangeSpriteState* state);

private:
	const s2::Skeleton* m_sk;

	Joint* m_selected;

	ee::ArrangeSpriteState* m_op_state;

}; // EditJointPoseOP

}

#endif // _LIBSKELETON_EDIT_JOINT_POSE_OP_H_
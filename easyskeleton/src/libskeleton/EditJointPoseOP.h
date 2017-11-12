#ifndef _LIBSKELETON_EDIT_JOINT_POSE_OP_H_
#define _LIBSKELETON_EDIT_JOINT_POSE_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Vector.h>

#include <memory>

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

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

private:
	bool Select(const sm::vec2& pos);

	void ChangeOPState(ee::ArrangeSpriteState* state);

private:
	const s2::Skeleton& m_sk;

	std::shared_ptr<Joint> m_selected;

	ee::ArrangeSpriteState* m_op_state;

}; // EditJointPoseOP

}

#endif // _LIBSKELETON_EDIT_JOINT_POSE_OP_H_
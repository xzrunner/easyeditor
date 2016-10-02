#ifndef _EASYMESH_EDIT_SKELETON_OP_H_
#define _EASYMESH_EDIT_SKELETON_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Vector.h>

namespace ee { class ArrangeSpriteState; }
namespace s2 { class MeshJoint; }

namespace emesh
{

class StagePanel;

class EditSkeletonOP : public ee::ZoomViewOP
{
public:
	EditSkeletonOP(StagePanel* stage);
	virtual ~EditSkeletonOP();

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
	s2::MeshJoint* m_selected;

	ee::ArrangeSpriteState* m_op_state;

}; // EditSkeletonOP

}

#endif // _EASYMESH_EDIT_SKELETON_OP_H_
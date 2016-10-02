#ifndef _EASYMESH_CREATE_SKELETON_OP_H_
#define _EASYMESH_CREATE_SKELETON_OP_H_

#include <easyshape.h>

namespace s2 { class MeshJoint; }

namespace emesh
{

class StagePanel;
class Mesh;

class CreateSkeletonOP : public eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>
{
public:
	CreateSkeletonOP(StagePanel* stage);
	virtual ~CreateSkeletonOP();

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	void RefreshAll();
	
private:
	StagePanel* m_stage;

	s2::MeshJoint* m_selected;

}; // CreateSkeletonOP

}

#endif // _EASYMESH_CREATE_SKELETON_OP_H_
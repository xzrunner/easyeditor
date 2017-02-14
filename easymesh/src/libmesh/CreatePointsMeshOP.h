#ifndef _EASYMESH_CREATE_POINTS_MESH_OP_H_
#define _EASYMESH_CREATE_POINTS_MESH_OP_H_

#include <easyshape.h>

namespace emesh
{

class StagePanel;

class CreatePointsMeshOP : public eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>
{
public:
	CreatePointsMeshOP(StagePanel* stage);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	StagePanel* m_stage;

	int m_selected_inner;

}; // CreatePointsMeshOP

}

#endif // _EASYMESH_CREATE_POINTS_MESH_OP_H_
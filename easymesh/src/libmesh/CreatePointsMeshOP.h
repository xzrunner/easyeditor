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

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

private:
	StagePanel* m_stage;

	int m_selected_inner;

}; // CreatePointsMeshOP

}

#endif // _EASYMESH_CREATE_POINTS_MESH_OP_H_
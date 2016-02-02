#ifndef _EASYMESH_CREATE_MESH_OP_H_
#define _EASYMESH_CREATE_MESH_OP_H_

#include <easyshape.h>

namespace emesh
{

class StagePanel;
class Shape;

class CreateMeshOP : public eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>
{
public:
	CreateMeshOP(StagePanel* stage);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);

	virtual bool OnDraw() const;

private:
	StagePanel* m_stage;

}; // CreateMeshOP

}

#endif // _EASYMESH_CREATE_MESH_OP_H_
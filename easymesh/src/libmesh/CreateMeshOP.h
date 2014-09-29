#ifndef _EASYMESH_CREATE_MESH_OP_H_
#define _EASYMESH_CREATE_MESH_OP_H_

#include <drag2d.h>
#include <easyshape.h>

namespace emesh
{

class StagePanel;
class Shape;

class CreateMeshOP : public libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>
{
public:
	CreateMeshOP(StagePanel* stage);

	virtual bool onKeyDown(int keyCode);
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);

	virtual bool onDraw() const;

private:
	StagePanel* m_stage;

}; // CreateMeshOP

}

#endif // _EASYMESH_CREATE_MESH_OP_H_
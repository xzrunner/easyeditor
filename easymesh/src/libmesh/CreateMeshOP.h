#ifndef _EASYMESH_CREATE_MESH_OP_H_
#define _EASYMESH_CREATE_MESH_OP_H_

#include <drag2d.h>
#include <easyshape.h>

namespace emesh
{

class StagePanel;
class Shape;

class CreateMeshOP : public libshape::EditPolylineOP<libshape::DrawLoopOP, d2d::SelectShapesOP>
{
public:
	CreateMeshOP(StagePanel* stage);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw(const d2d::Screen& scr) const;

private:
	StagePanel* m_stage;

	d2d::Vector* m_selected;

	d2d::Vector m_last_right;

}; // CreateMeshOP

}

#endif // _EASYMESH_CREATE_MESH_OP_H_
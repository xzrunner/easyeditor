#ifndef _EASYMESH_CREATE_MESH_OP_H_
#define _EASYMESH_CREATE_MESH_OP_H_

#include <drag2d.h>

namespace emesh
{

class StagePanel;
class CreateMeshOP : public d2d::ZoomViewOP
{
public:
	CreateMeshOP(StagePanel* stage);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;

private:
	StagePanel* m_stage;

	d2d::Vector* m_selected;

}; // CreateMeshOP

}

#endif // _EASYMESH_CREATE_MESH_OP_H_
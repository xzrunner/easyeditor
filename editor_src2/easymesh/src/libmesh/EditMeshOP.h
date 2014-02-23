#ifndef _EASYMESH_EDIT_MESH_OP_H_
#define _EASYMESH_EDIT_MESH_OP_H_

#include <drag2d.h>

namespace emesh
{

class Node;
class StagePanel;
class EditMeshOP : public d2d::ZoomViewOP
{
public:
	EditMeshOP(StagePanel* stage);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;

private:
	StagePanel* m_stage;

	Node* m_selected;

}; // EditMeshOP

}

#endif // _EASYMESH_EDIT_MESH_OP_H_
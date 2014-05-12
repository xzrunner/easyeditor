#ifndef _EASYMESH_EDIT_MESH_OP_H_
#define _EASYMESH_EDIT_MESH_OP_H_

#include "SelectNodesOP.h"

namespace emesh
{

class Node;
class StagePanel;
class EditMeshOP : public SelectNodesOP
{
public:
	EditMeshOP(StagePanel* stage);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;

private:
	void translasteNode(const d2d::Vector& offset);
	void rotateNode(const d2d::Vector& dst);

private:
	StagePanel* m_stage;

	d2d::Vector m_lastPos;

	bool m_bRightPress;
	d2d::Vector m_center;

	bool m_selCenter;

}; // EditMeshOP

}

#endif // _EASYMESH_EDIT_MESH_OP_H_
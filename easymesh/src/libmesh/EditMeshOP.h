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

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;

private:
	void translasteNode(const ee::Vector& offset);
	void rotateNode(const ee::Vector& dst);

private:
	StagePanel* m_stage;

	ee::Vector m_lastPos;

	bool m_bRightPress;
	ee::Vector m_center;

	bool m_selCenter;

}; // EditMeshOP

}

#endif // _EASYMESH_EDIT_MESH_OP_H_
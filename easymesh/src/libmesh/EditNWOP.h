#ifndef _EASYMESH_EDIT_NW_OP_H_
#define _EASYMESH_EDIT_NW_OP_H_

#include "SelectNodesOP.h"

namespace emesh
{

class Node;
class StagePanel;
class EditNWOP : public SelectNodesOP
{
public:
	EditNWOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;

private:
	void TranslasteNode(const ee::Vector& offset);
	void RotateNode(const ee::Vector& dst);

private:
	ee::Vector m_last_pos;

	bool m_right_press;
	ee::Vector m_center;

	bool m_select_center;

	bool m_dragable;

}; // EditNWOP

}

#endif // _EASYMESH_EDIT_NW_OP_H_
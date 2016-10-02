#ifndef _EASYMESH_EDIT_NETWORK_OP_H_
#define _EASYMESH_EDIT_NETWORK_OP_H_

#include "SelectNodesOP.h"

namespace emesh
{

class StagePanel;
class EditNetworkOP : public SelectNodesOP
{
public:
	EditNetworkOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;

private:
	void TranslasteNode(const sm::vec2& offset);
	void RotateNode(const sm::vec2& dst);

private:
	sm::vec2 m_last_pos;

	bool m_right_press;
	sm::vec2 m_center;

	bool m_select_center;

	bool m_dragable;

}; // EditNetworkOP

}

#endif // _EASYMESH_EDIT_NETWORK_OP_H_
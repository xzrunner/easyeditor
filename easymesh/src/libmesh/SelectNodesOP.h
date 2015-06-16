#ifndef _EASYMESH_SELECT_NODE_OP_H_
#define _EASYMESH_SELECT_NODE_OP_H_

#include "Node.h"

namespace emesh
{

class Mesh;
class StagePanel;

class SelectNodesOP : public d2d::DrawRectangleOP
{
public:
	SelectNodesOP(StagePanel* stage);
	virtual ~SelectNodesOP();

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

protected:
	NodeSelection m_selection;

private:
	StagePanel* m_stage;

	d2d::Vector m_firstPos;

	// To disable mouse able when press ctrl and window query
	bool m_bDraggable;

}; // SelectNodesOP

}

#endif // _EASYMESH_SELECT_NODE_OP_H_
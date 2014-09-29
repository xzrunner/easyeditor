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

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;
	virtual bool clear();

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
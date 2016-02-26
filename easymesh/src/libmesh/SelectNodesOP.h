#ifndef _EASYMESH_SELECT_NODE_OP_H_
#define _EASYMESH_SELECT_NODE_OP_H_

#include "Node.h"

#include <ee/DrawRectangleOP.h>

namespace emesh
{

class Mesh;
class StagePanel;

class SelectNodesOP : public ee::DrawRectangleOP
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
	ee::Vector m_first_pos;

	// To disable mouse able when press ctrl and window query
	bool m_draggable;

}; // SelectNodesOP

}

#endif // _EASYMESH_SELECT_NODE_OP_H_
#ifndef _EASYMESH_SELECT_NODE_OP_H_
#define _EASYMESH_SELECT_NODE_OP_H_

#include <ee/DrawRectangleOP.h>
#include <ee/SelectionSet.h>

#include <sprite2/MeshNode.h>

namespace emesh
{

class Network;
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
	ee::SelectionSet<s2::MeshNode> m_selection;

private:
	sm::vec2 m_first_pos;

	// To disable mouse able when press ctrl and window query
	bool m_draggable;

}; // SelectNodesOP

}

#endif // _EASYMESH_SELECT_NODE_OP_H_
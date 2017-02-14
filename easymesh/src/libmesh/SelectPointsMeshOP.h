#ifndef _EASYMESH_SELECT_VERTICES_OP_H_
#define _EASYMESH_SELECT_VERTICES_OP_H_

#include <ee/DrawRectangleOP.h>
#include <ee/SelectionSet.h>

#include <set>

namespace emesh
{

class PointsMesh;
class StagePanel;

class SelectPointsMeshOP : public ee::DrawRectangleOP
{
public:
	SelectPointsMeshOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

protected:
	std::set<int> m_selection;

private:
	sm::vec2 m_first_pos;

	// To disable mouse able when press ctrl and window query
	bool m_draggable;

}; // SelectNodesOP

}

#endif // _EASYMESH_SELECT_VERTICES_OP_H_
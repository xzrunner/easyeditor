#ifndef _EASYMESH_CREATE_NW_OP_H_
#define _EASYMESH_CREATE_NW_OP_H_

#include <easyshape.h>

namespace emesh
{

class StagePanel;
class Mesh;

class CreateNWOP : public eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>
{
public:
	CreateNWOP(StagePanel* stage);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;

private:
	void RefreshAll();

private:
	StagePanel* m_stage;

	ee::Vector* m_selected_inner;

}; // CreateNWOP

}

#endif // _EASYMESH_CREATE_NW_OP_H_
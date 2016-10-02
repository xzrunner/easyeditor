#ifndef _EASYMESH_CREATE_NETWORK_OP_H_
#define _EASYMESH_CREATE_NETWORK_OP_H_

#include <easyshape.h>

namespace emesh
{

class StagePanel;
class Mesh;

class CreateNetworkOP : public eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>
{
public:
	CreateNetworkOP(StagePanel* stage);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	void RefreshAll();

private:
	StagePanel* m_stage;

	sm::vec2* m_selected_inner;

}; // CreateNetworkOP

}

#endif // _EASYMESH_CREATE_NETWORK_OP_H_
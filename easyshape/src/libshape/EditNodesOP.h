#ifndef _EASYSHAPE_EDIT_NODES_OP_H_
#define _EASYSHAPE_EDIT_NODES_OP_H_

#include "SelectNodesOP.h"

namespace eshape
{

class EditNodesOP : public SelectNodesOP
{
public:
	EditNodesOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		ee::MultiShapesImpl* shapes_impl);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void Simplify(float threshold);
	void Smooth(float samplingWidth);
	void UpdateModified();

private:
	struct Modified
	{
		SelectNodesOP::ChainSelectedNodes* src;
		std::vector<sm::vec2> dst;
	};

private:
	sm::vec2 m_last_pos;

	std::vector<Modified> m_buffer;

}; // EditNodesOP

}

#endif // _EASYSHAPE_EDIT_NODES_OP_H_
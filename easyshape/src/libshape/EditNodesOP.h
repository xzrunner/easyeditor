#ifndef _EASYSHAPE_EDIT_NODES_OP_H_
#define _EASYSHAPE_EDIT_NODES_OP_H_

#include "SelectNodesOP.h"

namespace eshape
{

class EditNodesOP : public SelectNodesOP
{
public:
	EditNodesOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		ee::MultiShapesImpl* shapesImpl);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void simplify(float threshold);
	void smooth(float samplingWidth);
	void updateModified();

private:
	struct Modified
	{
		SelectNodesOP::ChainSelectedNodes* src;
		std::vector<ee::Vector> dst;
	};

private:
	ee::Vector m_lastPos;

	std::vector<Modified> m_buffer;

}; // EditNodesOP

}

#endif // _EASYSHAPE_EDIT_NODES_OP_H_
#ifndef _LIBSHAPE_EDIT_NODES_OP_H_
#define _LIBSHAPE_EDIT_NODES_OP_H_

#include "SelectNodesOP.h"

namespace libshape
{

class EditNodesOP : public SelectNodesOP
{
public:
	EditNodesOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl);

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
		std::vector<d2d::Vector> dst;
	};

private:
	d2d::Vector m_lastPos;

	std::vector<Modified> m_buffer;

}; // EditNodesOP

}

#endif // _LIBSHAPE_EDIT_NODES_OP_H_
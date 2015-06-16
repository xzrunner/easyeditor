#ifndef _EASYMESH_CREATE_STRIP_OP_H_
#define _EASYMESH_CREATE_STRIP_OP_H_

#include <drag2d.h>

namespace emesh
{

class StagePanel;

class CreateStripOP : public d2d::ZoomViewOP
{
public:
	CreateStripOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;

private:
	StagePanel* m_stage;

	d2d::Vector* m_selected;

	d2d::Vector m_last_right;

}; // CreateStripOP

}

#endif // _EASYMESH_CREATE_STRIP_OP_H_
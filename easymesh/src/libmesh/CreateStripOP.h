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

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;

private:
	StagePanel* m_stage;

	d2d::Vector* m_selected;

	d2d::Vector m_last_right;

}; // CreateStripOP

}

#endif // _EASYMESH_CREATE_STRIP_OP_H_
#ifndef _EASYMESH_CREATE_STRIP_OP_H_
#define _EASYMESH_CREATE_STRIP_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Vector.h>

namespace emesh
{

class StagePanel;

class CreateStripOP : public ee::ZoomViewOP
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

	sm::vec2* m_selected;

	sm::vec2 m_last_right;
	bool m_last_right_valid;

}; // CreateStripOP

}

#endif // _EASYMESH_CREATE_STRIP_OP_H_
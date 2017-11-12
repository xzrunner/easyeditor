#ifndef _EASYICON_EDIT_RECT_OP_H_
#define _EASYICON_EDIT_RECT_OP_H_

#include <ee/ZoomViewOP.h>

namespace eicon
{

class RectIcon;
class StagePanel;

class EditRectOP : public ee::ZoomViewOP
{
public:
	EditRectOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnActive() override;

	virtual bool OnDraw() const override;

private:
	static const float CTRL_NODE_RADIUS;

	enum PosType
	{
		PT_NULL = 0,
		PT_LEFT_LOW,
		PT_LEFT_TOP,
		PT_RIGHT_TOP,
		PT_RIGHT_LOW
	};

private:
	PosType m_selected;

}; // EditRectOP

}

#endif // _EASYICON_EDIT_RECT_OP_H_
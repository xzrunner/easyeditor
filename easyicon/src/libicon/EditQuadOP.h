#ifndef _EASYICON_EDIT_QUAD_OP_H_
#define _EASYICON_EDIT_QUAD_OP_H_

#include <ee/ZoomViewOP.h>

namespace eicon
{

class StagePanel;

class EditQuadOP : public ee::ZoomViewOP
{
public:
	EditQuadOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnActive() override;

	virtual bool OnDraw() const override;

private:
	static const float CTRL_NODE_RADIUS;

private:
	int m_selected;

}; // EditQuadOP

}

#endif // _EASYICON_EDIT_QUAD_OP_H_
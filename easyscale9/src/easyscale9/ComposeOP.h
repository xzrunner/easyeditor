#ifndef _EASYSCALE9_COMPOSE_OP_H_
#define _EASYSCALE9_COMPOSE_OP_H_

#include <drag2d.h>

namespace escale9
{

class StagePanel;
class ToolbarPanel;

class ComposeOP : public d2d::ZoomViewOP
{
public:
	ComposeOP(StagePanel* stage, ToolbarPanel* toolbar);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);

	virtual bool OnActive();

	virtual bool OnDraw() const;

private:
	d2d::ISprite* SelectByPos(int x, int y);

private:
	ToolbarPanel* m_toolbar;

}; // ComposeOP

}

#endif // _EASYSCALE9_COMPOSE_OP_H_
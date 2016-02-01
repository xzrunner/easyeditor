#ifndef _EASYSCALE9_COMPOSE_OP_H_
#define _EASYSCALE9_COMPOSE_OP_H_

#include <ee/ZoomViewOP.h>

namespace ee { class Sprite; }

namespace escale9
{

class StagePanel;
class ToolbarPanel;

class ComposeOP : public ee::ZoomViewOP
{
public:
	ComposeOP(StagePanel* stage, ToolbarPanel* toolbar);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);

	virtual bool OnActive();

	virtual bool OnDraw() const;

private:
	ee::Sprite* SelectByPos(int x, int y);

private:
	ToolbarPanel* m_toolbar;

}; // ComposeOP

}

#endif // _EASYSCALE9_COMPOSE_OP_H_
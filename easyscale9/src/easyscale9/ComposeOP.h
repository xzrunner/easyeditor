#ifndef _EASYSCALE9_COMPOSE_OP_H_
#define _EASYSCALE9_COMPOSE_OP_H_

#include <ee/ZoomViewOP.h>
#include <ee/Sprite.h>

namespace escale9
{

class StagePanel;
class ToolbarPanel;

class ComposeOP : public ee::ZoomViewOP
{
public:
	ComposeOP(StagePanel* stage, ToolbarPanel* toolbar);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;

	virtual bool OnActive() override;

	virtual bool OnDraw() const override;

private:
	ee::SprPtr SelectByPos(int x, int y);

private:
	ToolbarPanel* m_toolbar;

}; // ComposeOP

}

#endif // _EASYSCALE9_COMPOSE_OP_H_
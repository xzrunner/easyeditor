#ifndef _EASYSCALE9_RESIZE_OP_H_
#define _EASYSCALE9_RESIZE_OP_H_

#include <easyscale9.h>

namespace escale9
{

class StagePanel;
class ToolbarPanel;
class ResizeCMPT;

class ResizeOP : public escale9::ResizeBaseOP
{
public:
	ResizeOP(StagePanel* stage, ToolbarPanel* toolbar,
		ResizeCMPT* resizeCmpt);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnActive() override;

	virtual bool OnDraw() const override;

private:
	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;
	ResizeCMPT* m_resizeCmpt;

}; // ResizeOP

}

#endif // _EASYSCALE9_RESIZE_OP_H_
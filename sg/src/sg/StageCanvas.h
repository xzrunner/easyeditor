#ifndef _SG_STAGE_CANVAS_H_
#define _SG_STAGE_CANVAS_H_

#include <drag2d.h>

#include "SymbolRender.h"

namespace sg
{

class StagePanel;

class StageCanvas : public d2d::SpriteStageCanvas
{
public:
	StageCanvas(StagePanel* parent);
	virtual ~StageCanvas();

	void setBackground(d2d::ISymbol* background);

	static d2d::Vector transToBirdView(const d2d::Vector& pos);
	static d2d::Vector transToFlatView(const d2d::Vector& pos);

protected:
	virtual void onDraw();

	void onTimer(wxTimerEvent& event);

private:
	void drawBackground() const;
	void drawGuideLines() const;
	void drawGrass() const;
	void drawGrids() const;
	void drawArrow() const;

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	StagePanel* m_stage;

	d2d::ISprite* m_background;

	SymbolRender m_render;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _SG_STAGE_CANVAS_H_
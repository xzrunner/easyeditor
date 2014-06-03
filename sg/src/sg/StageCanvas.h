#ifndef _SG_STAGE_CANVAS_H_
#define _SG_STAGE_CANVAS_H_

#include <drag2d.h>

namespace sg
{

class StagePanel;

class StageCanvas : public d2d::SpriteStageCanvas
{
public:
	StageCanvas(StagePanel* stage);
	virtual ~StageCanvas();

	void SetBackground(d2d::ISymbol* background);

	static d2d::Vector TransToBirdView(const d2d::Vector& pos);
	static d2d::Vector TransToFlatView(const d2d::Vector& pos);

protected:
	virtual void onDraw();

	void OnTimer(wxTimerEvent& event);

private:
	void DrawBackground() const;
	void DrawGuideLines() const;
	void DrawGrass() const;
	void DrawGrids() const;
	void DrawSprites() const;
	void DrawArrow() const;

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	StagePanel* m_stage;

	d2d::ISprite* m_background;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _SG_STAGE_CANVAS_H_
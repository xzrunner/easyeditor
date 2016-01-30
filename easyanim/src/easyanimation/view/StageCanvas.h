#ifndef _EASYANIM_STAGE_CANVAS_H_
#define _EASYANIM_STAGE_CANVAS_H_

#include <drag2d.h>

namespace eanim
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);
	virtual ~StageCanvas();

	void SetBackground(d2d::Symbol* symbol);

protected:
	virtual void OnDrawSprites() const;

	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);

private:
	void DrawBackground() const;

private:
	StagePanel* m_stage_panel;

	d2d::Symbol* m_background;

}; // StageCanvas

}

#endif // _EASYANIM_STAGE_CANVAS_H_
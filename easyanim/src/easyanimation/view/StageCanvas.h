#ifndef _EASYANIM_STAGE_CANVAS_H_
#define _EASYANIM_STAGE_CANVAS_H_

#include <drag2d.h>

namespace eanim
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(d2d::EditPanel* stage);
	virtual ~StageCanvas();

	void SetBackground(d2d::ISymbol* symbol);

protected:
	virtual void onDraw();

	void onMouse(wxMouseEvent& event);
	void onKeyDown(wxKeyEvent& event);

private:
	void drawbackground() const;

private:
	d2d::ISymbol* m_background;

}; // StageCanvas

}

#endif // _EASYANIM_STAGE_CANVAS_H_
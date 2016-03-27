#ifndef _EASYANIM_STAGE_CANVAS_H_
#define _EASYANIM_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace ee { class Symbol; }

namespace eanim
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);
	virtual ~StageCanvas();

	void SetBackground(ee::Symbol* symbol);

protected:
	virtual void OnDrawSprites() const;

	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);

private:
	void DrawBackground() const;

private:
	StagePanel* m_stage_panel;

	ee::Symbol* m_background;

}; // StageCanvas

}

#endif // _EASYANIM_STAGE_CANVAS_H_
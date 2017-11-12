#ifndef _EASYANIM_STAGE_CANVAS_H_
#define _EASYANIM_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>
#include <ee/Symbol.h>

namespace eanim
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);

	void SetBackground(const ee::SymPtr& sym) {
		m_background = sym;
	}

protected:
	virtual void OnDrawSprites() const override;

	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);

private:
	void DrawBackground() const;

private:
	StagePanel* m_stage_panel;

	ee::SymPtr m_background = nullptr;

}; // StageCanvas

}

#endif // _EASYANIM_STAGE_CANVAS_H_
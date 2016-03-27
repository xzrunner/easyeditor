#ifndef _EASYSCALE9_STAGE_CANVAS_H_
#define _EASYSCALE9_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace escale9
{

class StagePanel;
class ToolbarPanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* editPanel);
	virtual ~StageCanvas();

	void setToolbarPanel(ToolbarPanel* toolbar) {
		m_toolbar = toolbar;
	}

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawGuideLines() const;

private:
	StagePanel* m_stage_panel;

	ToolbarPanel* m_toolbar;

}; // StageCanvas

}

#endif // _EASYSCALE9_STAGE_CANVAS_H_
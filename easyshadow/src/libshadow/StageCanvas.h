#ifndef _EASYSHADOW_STAGE_CANVAS_H_
#define _EASYSHADOW_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eshadow
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* panel);

protected:
	virtual void onDraw();

private:
	StagePanel* m_panel;

}; // StageCanvas

}

#endif // _EASYSHADOW_STAGE_CANVAS_H_
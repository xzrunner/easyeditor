#ifndef _EASYTEMPLATE_STAGE_CANVAS_H_
#define _EASYTEMPLATE_STAGE_CANVAS_H_

#include "drag2d.h"

namespace etemplate
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

#endif // _EASYTEMPLATE_STAGE_CANVAS_H_
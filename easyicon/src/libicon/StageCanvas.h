#ifndef _EASYICON_STAGE_CANVAS_H_
#define _EASYICON_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eicon
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void onDraw();

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYICON_STAGE_CANVAS_H_

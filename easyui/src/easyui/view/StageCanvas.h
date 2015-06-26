#ifndef _EASYUI_STAGE_CANVAS_H_
#define _EASYUI_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eui
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDraw();

private:
	void DrawGuideLines() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYUI_STAGE_CANVAS_H_

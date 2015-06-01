#ifndef _EASYUI_STAGE_CANVAS_H_
#define _EASYUI_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eui
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* statge);

protected:
	virtual void onDraw();

private:
	void DrawGuideLines() const;

private:
	StagePanel* m_statge;

}; // StageCanvas

}

#endif // _EASYUI_STAGE_CANVAS_H_

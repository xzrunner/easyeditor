#ifndef _EASYMESH_STAGE_CANVAS_H_
#define _EASYMESH_STAGE_CANVAS_H_

#include "drag2d.h"

namespace emesh
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* panel);

protected:
	virtual void initGL();
	virtual void OnDraw();

private:
	StagePanel* m_panel;

}; // StageCanvas

}

#endif // _EASYMESH_STAGE_CANVAS_H_
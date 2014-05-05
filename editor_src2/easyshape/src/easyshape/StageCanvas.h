#ifndef _ESHAPE_STAGE_CANVAS_H_
#define _ESHAPE_STAGE_CANVAS_H_

#include <drag2d.h>

namespace eshape
{

class StagePanel;

class StageCanvas : public d2d::ShapeStageCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void onDraw();

private:
	void drawGuideLines();

private:
	StagePanel* m_stage;

}; // StageCanvas 

}

#endif // _ESHAPE_STAGE_CANVAS_H_
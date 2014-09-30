#ifndef _EASYSKETCH_STAGE_CANVAS_H_
#define _EASYSKETCH_STAGE_CANVAS_H_

#include <drag2d.h>

namespace esketch
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void onDraw();

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYSKETCH_STAGE_CANVAS_H_

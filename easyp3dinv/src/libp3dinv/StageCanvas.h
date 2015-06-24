#ifndef _EASYP3DINV_STAGE_CANVAS_H_
#define _EASYP3DINV_STAGE_CANVAS_H_

#include <drag2d.h>

namespace ep3dinv
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDraw() const;

	virtual void OnTimer();

private:
	StagePanel* m_stage;

	int m_currFrame;
	clock_t m_last;

	d2d::PlayControl m_control;

}; // StageCanvas

}

#endif // _EASYP3DINV_STAGE_CANVAS_H_
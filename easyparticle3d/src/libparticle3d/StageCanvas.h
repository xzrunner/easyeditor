#ifndef _EASYPARTICLE3D_STAGE_CANVAS_H_
#define _EASYPARTICLE3D_STAGE_CANVAS_H_

#include <drag2d.h>
#include <easyanim.h>

namespace eparticle3d
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* editPanel);
	virtual ~StageCanvas();

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	static const int FRAME_RATE = 60;

private:
	StagePanel* m_stage;

	int m_currFrame;
	clock_t m_last;

	d2d::PlayControl m_control;

}; // StageCanvas

}

#endif // _EASYPARTICLE3D_STAGE_CANVAS_H_

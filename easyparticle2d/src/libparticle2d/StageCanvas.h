#ifndef _EASYPARTICLE2D_STAGE_CANVAS_H_
#define _EASYPARTICLE2D_STAGE_CANVAS_H_

#include <drag2d.h>
#include <easyanim.h>

namespace eparticle2d
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* stage);
	virtual ~StageCanvas();

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	void DrawBackground() const;

private:
	static const int FRAME_RATE = 60;

private:
	// todo rm, only UpdateSymbol
	void UpdateParticle2d();
	void UpdateAnimation();

private:
	StagePanel* m_stage;

	int m_currFrame;

	d2d::PlayControl m_control;

}; // StageCanvas

}

#endif // _EASYPARTICLE2D_STAGE_CANVAS_H_

#ifndef _EASYTEXPACKER_STAGE_CANVAS_H_
#define _EASYTEXPACKER_STAGE_CANVAS_H_

#include <drag2d.h>

namespace etp
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawRegion() const;

private:
	StagePanel* m_stage_panel;

	d2d::MultiSpritesImpl* m_stage_impl;

}; // StageCanvas

}

#endif // _EASYTEXPACKER_STAGE_CANVAS_H_
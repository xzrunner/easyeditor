#ifndef _EASYTEXPACKER_STAGE_CANVAS_H_
#define _EASYTEXPACKER_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace ee { class MultiSpritesImpl; }

namespace etexpacker
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawRegion() const;

private:
	StagePanel* m_stage_panel;

	ee::MultiSpritesImpl* m_stage_impl;

}; // StageCanvas

}

#endif // _EASYTEXPACKER_STAGE_CANVAS_H_
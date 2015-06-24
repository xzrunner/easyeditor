#ifndef _EASYSHADOW_STAGE_CANVAS_H_
#define _EASYSHADOW_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eshadow
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* stage);
	StageCanvas(StagePanel* stage, d2d::ISprite* edited,
		const std::vector<d2d::ISprite*>& bg_sprites);

protected:
	virtual void initGL();
	virtual void OnDraw();

private:
	StagePanel* m_stage;

	d2d::ISprite* m_edited;
	std::vector<d2d::ISprite*> m_bg_sprites;

}; // StageCanvas

}

#endif // _EASYSHADOW_STAGE_CANVAS_H_
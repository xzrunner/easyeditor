#ifndef _EASYTEXTURE_STAGE_CANVAS_H_
#define _EASYTEXTURE_STAGE_CANVAS_H_

#include "drag2d.h"

namespace etexture
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* panel);
	StageCanvas(StagePanel* panel, d2d::ISprite* edited,
		const std::vector<d2d::ISprite*>& bg_sprites);

protected:
	virtual void InitGL();
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_panel;

	d2d::ISprite* m_edited;
	std::vector<d2d::ISprite*> m_bg_sprites;

}; // StageCanvas

}

#endif // _EASYTEXTURE_STAGE_CANVAS_H_

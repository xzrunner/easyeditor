#ifndef _EASYTEXTURE_STAGE_CANVAS_H_
#define _EASYTEXTURE_STAGE_CANVAS_H_

#include "drag2d.h"

namespace etexture
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* panel);
	StageCanvas(StagePanel* panel, wxGLContext* glctx,
		d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites);

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_panel;

	d2d::ISprite* m_edited;
	const d2d::MultiSpritesImpl* m_sprite_impl;

	d2d::ISprite* m_bg;

}; // StageCanvas

}

#endif // _EASYTEXTURE_STAGE_CANVAS_H_

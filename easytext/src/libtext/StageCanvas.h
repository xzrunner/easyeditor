#ifndef _EASYTEXT_STAGE_CANVAS_H_
#define _EASYTEXT_STAGE_CANVAS_H_

#include <drag2d.h>

namespace etext
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);
	StageCanvas(StagePanel* stage, wxGLContext* glctx,
		d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawSprBound() const;

private:
	StagePanel* m_stage;

	d2d::ISprite* m_edited;
	const d2d::MultiSpritesImpl* m_sprite_impl;

	d2d::ISprite* m_bg;

}; // StageCanvas

}

#endif // _EASYTEXT_STAGE_CANVAS_H_
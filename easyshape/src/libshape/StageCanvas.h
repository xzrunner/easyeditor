#ifndef _LIBSHAPE_STAGE_CANVAS_H_
#define _LIBSHAPE_STAGE_CANVAS_H_

#include <drag2d.h>

namespace libshape
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);
	StageCanvas(StagePanel* stage, wxGLContext* glctx,
		d2d::Sprite* edited, const d2d::MultiSpritesImpl* bg_sprites);

public:
	void DrawGuideLines() const;

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_stage;

	const d2d::MultiShapesImpl* m_shape_impl;

	d2d::Sprite* m_edited;
	const d2d::MultiSpritesImpl* m_sprite_impl;

	d2d::Sprite* m_bg;

}; // StageCanvas

}

#endif // _LIBSHAPE_STAGE_CANVAS_H_
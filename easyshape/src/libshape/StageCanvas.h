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
	StageCanvas(StagePanel* stage, d2d::ISprite* edited,
		const d2d::MultiSpritesImpl* bg_sprites);

public:
	void DrawGuideLines() const;

protected:
	virtual void InitGL();
	virtual void OnDrawSprites() const;

private:
	const d2d::MultiShapesImpl* m_shape_impl;

	d2d::ISprite* m_edited;
	const d2d::MultiSpritesImpl* m_sprite_impl;

	d2d::ISprite* m_bg;

}; // StageCanvas

}

#endif // _LIBSHAPE_STAGE_CANVAS_H_
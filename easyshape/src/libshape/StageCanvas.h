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
		const std::vector<d2d::ISprite*>& bg_sprites);

public:
	void DrawGuideLines() const;

protected:
	virtual void InitGL();
	virtual void OnDrawSprites() const;

private:
	d2d::ISprite* m_edited;
	std::vector<d2d::ISprite*> m_bg_sprites;

	d2d::MultiShapesImpl* m_stage_impl;

}; // StageCanvas

}

#endif // _LIBSHAPE_STAGE_CANVAS_H_
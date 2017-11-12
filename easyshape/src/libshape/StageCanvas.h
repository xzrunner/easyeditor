#ifndef _EASYSHAPE_STAGE_CANVAS_H_
#define _EASYSHAPE_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>
#include <ee/Sprite.h>

namespace ee { class MultiSpritesImpl; class MultiShapesImpl; }

namespace eshape
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);
	StageCanvas(StagePanel* stage, wxGLContext* glctx,
		ee::SprPtr edited, const ee::MultiSpritesImpl* bg_sprites);

public:
	void DrawGuideLines() const;

protected:
	virtual void OnDrawSprites() const override;

private:
	StagePanel* m_stage;

	const ee::MultiShapesImpl* m_shape_impl;

	ee::SprPtr m_edited;
	const ee::MultiSpritesImpl* m_sprite_impl;

	ee::SprPtr m_bg;

}; // StageCanvas

}

#endif // _EASYSHAPE_STAGE_CANVAS_H_
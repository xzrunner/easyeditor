#ifndef _EASYICON_STAGE_CANVAS_H_
#define _EASYICON_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>
#include <ee/Sprite.h>

namespace ee { class MultiSpritesImpl; }

namespace eicon
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);
	StageCanvas(StagePanel* stage, wxGLContext* glctx,
		ee::SprPtr edited, const ee::MultiSpritesImpl* bg_sprites);

protected:
	virtual void OnDrawSprites() const override;

private:
	StagePanel* m_stage;

	ee::SprPtr m_edited;
	const ee::MultiSpritesImpl* m_sprite_impl;

	ee::SprPtr m_bg;

}; // StageCanvas

}

#endif // _EASYICON_STAGE_CANVAS_H_

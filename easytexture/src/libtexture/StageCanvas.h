#ifndef _EASYTEXTURE_STAGE_CANVAS_H_
#define _EASYTEXTURE_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>
#include <ee/Sprite.h>

namespace ee { class MultiSpritesImpl; }

namespace etexture
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* panel);
	StageCanvas(StagePanel* panel, wxGLContext* glctx, const ee::SprPtr& edited, 
		const ee::MultiSpritesImpl* bg_sprites);

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_panel;

	ee::SprPtr m_edited;
	const ee::MultiSpritesImpl* m_sprite_impl;

	ee::SprPtr m_bg;

}; // StageCanvas

}

#endif // _EASYTEXTURE_STAGE_CANVAS_H_

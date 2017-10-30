#ifndef _EASYMESH_STAGE_CANVAS_H_
#define _EASYMESH_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>
#include <ee/Sprite.h>

namespace ee { class MultiSpritesImpl; }

namespace emesh
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* panel, wxGLContext* glctx = NULL);
	StageCanvas(StagePanel* panel, wxGLContext* glctx,
		ee::SprPtr edited, const ee::MultiSpritesImpl* bg_sprites);

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_panel;

	ee::SprPtr m_edited;
	const ee::MultiSpritesImpl* m_sprite_impl;

	ee::SprPtr m_bg;

}; // StageCanvas

}

#endif // _EASYMESH_STAGE_CANVAS_H_
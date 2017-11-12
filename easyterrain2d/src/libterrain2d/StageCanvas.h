#ifndef _EASYTERRAIN2D_STAGE_CANVAS_H_
#define _EASYTERRAIN2D_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace ee { class Sprite; class MultiSpritesImpl; }

namespace eterrain2d
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* panel);
	StageCanvas(StagePanel* panel, wxGLContext* glctx,
		ee::SprPtr edited, const ee::MultiSpritesImpl* bg_sprites);
	virtual ~StageCanvas();

protected:
	virtual void OnDrawSprites() const override;

	virtual void OnTimer() override;

private:
	void DrawBG() const;

	void Update();

private:
	StagePanel* m_panel;

	ee::SprPtr m_edited;
	const ee::MultiSpritesImpl* m_sprite_impl;

	ee::SprPtr m_bg;

}; // StageCanvas

}

#endif // _EASYTERRAIN2D_STAGE_CANVAS_H_
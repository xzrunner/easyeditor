#ifndef _EASYTERRAIN2D_STAGE_CANVAS_H_
#define _EASYTERRAIN2D_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eterrain2d
{

class StagePanel;

class StageCanvas : public ee::OrthoCanvas
{
public:
	StageCanvas(StagePanel* panel);
	StageCanvas(StagePanel* panel, wxGLContext* glctx,
		ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites);

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	void DrawBG() const;

	void Update();

private:
	StagePanel* m_panel;

	ee::Sprite* m_edited;
	const ee::MultiSpritesImpl* m_sprite_impl;

	ee::Sprite* m_bg;

}; // StageCanvas

}

#endif // _EASYTERRAIN2D_STAGE_CANVAS_H_
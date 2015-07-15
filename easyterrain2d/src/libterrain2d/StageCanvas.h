#ifndef _EASYTERRAIN2D_STAGE_CANVAS_H_
#define _EASYTERRAIN2D_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eterrain2d
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* panel);
	StageCanvas(StagePanel* panel, d2d::ISprite* edited,
		const d2d::MultiSpritesImpl* bg_sprites);

protected:
	virtual void InitGL();
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	void DrawBG() const;

	void Update();

private:
	StagePanel* m_panel;

	d2d::ISprite* m_edited;
	const d2d::MultiSpritesImpl* m_sprite_impl;

	d2d::ISprite* m_bg;

}; // StageCanvas

}

#endif // _EASYTERRAIN2D_STAGE_CANVAS_H_
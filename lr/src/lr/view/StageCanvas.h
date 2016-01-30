#ifndef _LR_STAGE_CANVAS_H_
#define _LR_STAGE_CANVAS_H_

#include <drag2d.h>

namespace lr
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);

	d2d::Colorf& GetMultiColor() { return m_scr_style.multi_col; }
	d2d::Colorf& GetAddColor() { return m_scr_style.add_col; }

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawSprites() const;
	void DrawSprite(d2d::Sprite* spr, bool draw_edge) const;

	void DrawRegion() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _LR_STAGE_CANVAS_H_

#ifndef _LR_STAGE_CANVAS_H_
#define _LR_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace ee { class Sprite; }

namespace lr
{

class Layer;
class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);

	s2::Color& GetMultiColor() { return m_scr_style.multi_col; }
	s2::Color& GetAddColor() { return m_scr_style.add_col; }

	void EnableColGrading(bool enable) { m_scr_style.col_grading = enable; }

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawSprites() const;
	void DrawSprite(ee::Sprite* spr, bool draw_edge, int name_visible) const;

	void DrawRegion() const;
	void DrawPseudo3dBound() const;

	void DrawLayer(const Layer* layer) const;

private:
	struct DrawableSpr
	{
		ee::Sprite* spr;
		int name_visible;

		DrawableSpr(ee::Sprite* spr, int name_visible) 
			: spr(spr), name_visible(name_visible) {}
	};

	class SprCmp
	{
	public:
		bool operator() (const DrawableSpr& s0, const DrawableSpr& s1) const;
	}; // SprCmp

private:
	StagePanel* m_stage;

	mutable std::vector<sm::vec2> m_bound_pseudo3d;

}; // StageCanvas

}

#endif // _LR_STAGE_CANVAS_H_

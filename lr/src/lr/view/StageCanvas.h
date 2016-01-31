#ifndef _LR_STAGE_CANVAS_H_
#define _LR_STAGE_CANVAS_H_



namespace lr
{

class StagePanel;

class StageCanvas : public ee::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);

	ee::Colorf& GetMultiColor() { return m_scr_style.multi_col; }
	ee::Colorf& GetAddColor() { return m_scr_style.add_col; }

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawSprites() const;
	void DrawSprite(ee::Sprite* spr, bool draw_edge) const;

	void DrawRegion() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _LR_STAGE_CANVAS_H_

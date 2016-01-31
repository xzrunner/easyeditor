#ifndef _LIBSHAPE_STAGE_CANVAS_H_
#define _LIBSHAPE_STAGE_CANVAS_H_



namespace libshape
{

class StagePanel;

class StageCanvas : public ee::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);
	StageCanvas(StagePanel* stage, wxGLContext* glctx,
		ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites);

public:
	void DrawGuideLines() const;

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_stage;

	const ee::MultiShapesImpl* m_shape_impl;

	ee::Sprite* m_edited;
	const ee::MultiSpritesImpl* m_sprite_impl;

	ee::Sprite* m_bg;

}; // StageCanvas

}

#endif // _LIBSHAPE_STAGE_CANVAS_H_
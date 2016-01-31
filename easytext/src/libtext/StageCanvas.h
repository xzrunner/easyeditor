#ifndef _EASYTEXT_STAGE_CANVAS_H_
#define _EASYTEXT_STAGE_CANVAS_H_



namespace etext
{

class StagePanel;

class StageCanvas : public ee::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);
	StageCanvas(StagePanel* stage, wxGLContext* glctx,
		ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawSprBound() const;

private:
	StagePanel* m_stage;

	ee::Sprite* m_edited;
	const ee::MultiSpritesImpl* m_sprite_impl;

	ee::Sprite* m_bg;

}; // StageCanvas

}

#endif // _EASYTEXT_STAGE_CANVAS_H_
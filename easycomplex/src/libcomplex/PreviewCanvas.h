#ifndef _EASYCOMPLEX_PREVIEW_CANVAS_H_
#define _EASYCOMPLEX_PREVIEW_CANVAS_H_



namespace ecomplex
{

class PreviewCanvas : public ee::OrthoCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		const std::vector<const ee::Sprite*>& sprites, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

private:
	void drawStageData();

//	void getCurrSprites(std::vector<ee::Sprite*>& sprites) const;

private:
	std::vector<const ee::Sprite*> m_sprites;

}; // PreviewCanvas

}

#endif // _EASYCOMPLEX_PREVIEW_CANVAS_H_
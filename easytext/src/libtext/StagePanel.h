#ifndef _EASYTEXT_STAGE_PANEL_H_
#define _EASYTEXT_STAGE_PANEL_H_



namespace etext
{

class StageCanvas;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, ee::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, wxGLContext* glctx,
		ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites, 
		ee::LibraryPanel* library);
	virtual ~StagePanel();

private:
	StageCanvas* m_canvas;

}; // StagePanel

}

#endif // _EASYTEXT_STAGE_PANEL_H_
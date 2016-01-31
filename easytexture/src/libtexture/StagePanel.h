#ifndef _EASYTEXTURE_STAGE_PANEL_H_
#define _EASYTEXTURE_STAGE_PANEL_H_



namespace etexture
{

class Symbol;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl, 
	public ee::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		wxGLContext* glctx, ee::Sprite* edited, 
		const ee::MultiSpritesImpl* bg_sprites, ee::LibraryPanel* library);
	virtual ~StagePanel();

	Symbol* GetSymbol() { return m_symbol; }

private:
	Symbol* m_symbol;

}; // StagePanel

}

#endif // _EASYTEXTURE_STAGE_PANEL_H_

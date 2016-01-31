#ifndef _EASYICON_STAGE_PANEL_H_
#define _EASYICON_STAGE_PANEL_H_



namespace eicon
{

class Symbol;
class Icon;

class StagePanel : public ee::EditPanel
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		wxGLContext* glctx, ee::Sprite* edited, 
		const ee::MultiSpritesImpl* bg_sprites);
	virtual ~StagePanel();

	void SetIcon(Icon* icon);
	Icon* GetIcon();

	void SetImage(ee::Image* img);

	const Symbol& GetSymbol() const { return *m_symbol; }
	Symbol& GetSymbol() { return *m_symbol; }

private:
	class StageDropTarget : public ee::StageDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, ee::LibraryPanel* library);
		virtual bool OnDropSymbol(ee::Symbol* symbol, const ee::Vector& pos);
	private:
		StagePanel* m_stage;
	}; // StageDropTarget 

private:
	Symbol* m_symbol;

}; // StagePanel

}

#endif // _EASYICON_STAGE_PANEL_H_

#ifndef _EASYICON_STAGE_PANEL_H_
#define _EASYICON_STAGE_PANEL_H_

#include <drag2d.h>

namespace eicon
{

class Symbol;
class Icon;

class StagePanel : public d2d::EditPanel
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		wxGLContext* glctx, d2d::ISprite* edited, 
		const d2d::MultiSpritesImpl* bg_sprites);
	virtual ~StagePanel();

	void SetIcon(Icon* icon);
	Icon* GetIcon();

	void SetImage(d2d::Image* img);

	const Symbol& GetSymbol() const { return *m_symbol; }
	Symbol& GetSymbol() { return *m_symbol; }

private:
	class StageDropTarget : public d2d::StageDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library);
		virtual bool OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos);
	private:
		StagePanel* m_stage;
	}; // StageDropTarget 

private:
	Symbol* m_symbol;

}; // StagePanel

}

#endif // _EASYICON_STAGE_PANEL_H_

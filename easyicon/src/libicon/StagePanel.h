#ifndef _EASYICON_STAGE_PANEL_H_
#define _EASYICON_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/StageDropTarget.h>

class wxGLContext;

namespace ee { class LibraryPanel; class MultiSpritesImpl; class ImageSymbol; }

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

	void SetImage(ee::ImageSymbol* img);

	const Symbol& GetSymbol() const { return *m_sym; }
	Symbol& GetSymbol() { return *m_sym; }

private:
	class StageDropTarget : public ee::StageDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, ee::LibraryPanel* library);
		virtual bool OnDropSymbol(ee::Symbol* sym, const sm::vec2& pos);
	private:
		StagePanel* m_stage;
	}; // StageDropTarget 

private:
	Symbol* m_sym;

}; // StagePanel

}

#endif // _EASYICON_STAGE_PANEL_H_

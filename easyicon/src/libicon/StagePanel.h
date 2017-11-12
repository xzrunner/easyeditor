#ifndef _EASYICON_STAGE_PANEL_H_
#define _EASYICON_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/StageDropTarget.h>
#include <ee/Symbol.h>

#include <sprite2/Icon.h>

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
		wxGLContext* glctx, const ee::SprPtr& edited, 
		const ee::MultiSpritesImpl* bg_sprites);

	void SetIcon(std::unique_ptr<s2::Icon>& icon);
	Icon* GetIcon();

	void SetImage(const std::shared_ptr<ee::ImageSymbol>& img);

	const std::shared_ptr<Symbol>& GetSymbol() const { return m_sym; }
	std::shared_ptr<Symbol>& GetSymbol() { return m_sym; }

private:
	class StageDropTarget : public ee::StageDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, ee::LibraryPanel* library);
		virtual bool OnDropSymbol(const ee::SymPtr& sym, const sm::vec2& pos) override;
	private:
		StagePanel* m_stage;
	}; // StageDropTarget 

private:
	std::shared_ptr<Symbol> m_sym;

}; // StagePanel

}

#endif // _EASYICON_STAGE_PANEL_H_

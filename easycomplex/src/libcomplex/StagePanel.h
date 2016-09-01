#ifndef _EASYCOMPLEX_STAGE_PANEL_H_
#define _EASYCOMPLEX_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>

namespace ee { class PropertySettingPanel; class CrossGuides; }

class wxGLContext;

namespace ecomplex
{

class LibraryPanel;
class Symbol;
class ArrangeSpriteImpl;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		ee::PropertySettingPanel* property, LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, Symbol* sym, 
		ee::PropertySettingPanel* property, LibraryPanel* library, 
		wxGLContext* glctx, ee::CrossGuides* guides = NULL);

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage();

	const Symbol* GetSymbol() const { return m_sym; }
	Symbol* GetSymbol() { return m_sym; }

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void Clear();

private:
	Symbol* m_sym;

	LibraryPanel* m_library;

}; // StagePanel

}

#endif // _EASYCOMPLEX_STAGE_PANEL_H_
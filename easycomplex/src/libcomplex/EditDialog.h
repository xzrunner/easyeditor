#ifndef _EASYCOMPLEX_EDIT_DIALOG_H_
#define _EASYCOMPLEX_EDIT_DIALOG_H_

#include <wx/dialog.h>

class wxGLContext;

namespace ee { class LibraryPanel; class ViewlistPanel; class CrossGuides; }

namespace ecomplex
{

class Symbol;
class PropertySettingPanel;
class StagePanel;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, Symbol* symbol, 
		wxGLContext* glctx, ee::CrossGuides* guides = NULL);

private:
	void InitLayout(wxGLContext* glctx, ee::CrossGuides* guides);
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent, wxGLContext* glctx, ee::CrossGuides* guides);
	wxWindow* InitLayoutRight(wxWindow* parent);

	void OnCloseEvent(wxCloseEvent& event);

	void LoadSymbolInfo();

private:
	ee::LibraryPanel* m_library;
	PropertySettingPanel* m_property;
	StagePanel* m_stage;
	ee::ViewlistPanel* m_viewlist;

	Symbol* m_symbol;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYCOMPLEX_EDIT_DIALOG_H_
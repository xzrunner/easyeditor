#ifndef _DRAG2D_EDIT_DIALOG_H_
#define _DRAG2D_EDIT_DIALOG_H_

#include <drag2d.h>

namespace ecomplex
{

class Symbol;
class StagePanel;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, Symbol* symbol);

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

	void OnCloseEvent(wxCloseEvent& event);
	void OnClose(bool force, int returncode);

	void LoadSymbolInfo();

private:
	d2d::LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
	StagePanel* m_stage;
	d2d::ViewlistPanel* m_viewlist;

	Symbol* m_symbol;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _DRAG2D_EDIT_DIALOG_H_
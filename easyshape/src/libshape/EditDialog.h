#ifndef _EASYSHAPE_ETID_DIALOG_H_
#define _EASYSHAPE_ETID_DIALOG_H_

#include <wx/dialog.h>

namespace eshape
{

class StagePanel;
class ToolbarPanel;
class Symbol;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, Symbol* symbol);
	virtual ~EditDialog();

private:
	void onSize(wxSizeEvent& event);

	void InitLayout(Symbol* symbol);

	void OnCloseEvent(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	ToolbarPanel* m_toolbar;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYSHAPE_ETID_DIALOG_H_
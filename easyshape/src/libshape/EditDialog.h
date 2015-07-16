#ifndef _LIBSHAPE_ETID_DIALOG_H_
#define _LIBSHAPE_ETID_DIALOG_H_

//#include <drag2d.h>

#include <wx/wx.h>

namespace libshape
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

#endif // _LIBSHAPE_ETID_DIALOG_H_
#ifndef _LIBSHAPE_MAIN_DIALOG_H_
#define _LIBSHAPE_MAIN_DIALOG_H_

#include <wx/wx.h>

namespace libshape
{

class ToolbarPanel;
class Symbol;

template <typename TStage>
class MainDialog : public wxDialog
{
public:
	MainDialog(wxWindow* parent, Symbol* symbol);
	virtual ~MainDialog();

private:
	void onSize(wxSizeEvent& event);

	void InitLayout(Symbol* symbol);

private:
	wxWindow* m_root;

	TStage* m_stage;
	ToolbarPanel* m_toolbar;

}; // MainDialog

}

#include "MainDialog.inl"

#endif // _LIBSHAPE_MAIN_DIALOG_H_
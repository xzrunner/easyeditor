#ifndef _EASYSHAPE_ETID_DIALOG_H_
#define _EASYSHAPE_ETID_DIALOG_H_

#include <wx/dialog.h>

#include <memory>

namespace eshape
{

class StagePanel;
class ToolbarPanel;
class Symbol;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, const std::shared_ptr<Symbol>& sym);
	virtual ~EditDialog();

private:
	void onSize(wxSizeEvent& event);

	void InitLayout(const std::shared_ptr<Symbol>& sym);

	void OnCloseEvent(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	ToolbarPanel* m_toolbar;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYSHAPE_ETID_DIALOG_H_
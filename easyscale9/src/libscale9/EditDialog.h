#ifndef _EASYSCALE9_EDIT_DIALOG_H_
#define _EASYSCALE9_EDIT_DIALOG_H_

namespace ee { class EditPanel; }

#include <wx/dialog.h>

#include <memory>

class wxGLContext;

namespace escale9
{

class Symbol;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, const std::shared_ptr<Symbol>& sym, wxGLContext* glctx);

private:
	void InitLayout(wxGLContext* glctx);

	void StoreSymbol();

	void OnCloseEvent(wxCloseEvent& event);

private:
	std::shared_ptr<Symbol> m_sym;

	ee::EditPanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYSCALE9_EDIT_DIALOG_H_
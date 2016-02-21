#ifndef _EASYANIM_PREVIEW_DIALOG_H_
#define _EASYANIM_PREVIEW_DIALOG_H_

#include <wx/dialog.h>

class wxGLContext;

namespace ee { class EditPanel; }

namespace eanim
{

class Symbol;

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, const Symbol* symbol,
		wxGLContext* glctx);
	~PreviewDialog();

private:
	void InitLayout(wxGLContext* glctx);

	void BuildToolBar(wxSizer* topSizer);
	void BuildEditPanel(wxSizer* topSizer, wxGLContext* glctx);

	void OnSetCirculate(wxCommandEvent& event);
	void OnSetStop(wxCommandEvent& event);

private:
	ee::EditPanel* m_stage;

	const Symbol* m_symbol;

}; // PreviewDialog

}

#endif // _EASYANIM_PREVIEW_DIALOG_H_
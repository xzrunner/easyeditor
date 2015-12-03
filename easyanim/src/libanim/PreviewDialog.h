#ifndef _LIBANIM_PREVIEW_DIALOG_H_
#define _LIBANIM_PREVIEW_DIALOG_H_

#include <wx/wx.h>
#include <drag2d.h>

namespace libanim
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
	d2d::EditPanel* m_stage;

	const Symbol* m_symbol;

}; // PreviewDialog

}

#endif // _LIBANIM_PREVIEW_DIALOG_H_
#ifndef _EASYANIM_PREVIEW_DIALOG_H_
#define _EASYANIM_PREVIEW_DIALOG_H_

#include <ee/PlayControl.h>

#include <easyanim.h>

#include <wx/wx.h>

namespace ee { class EditPanel; }

class wxGLContext;

namespace eanim
{

class PreviewPanel;

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, wxGLContext* glctx, s2::AnimSymbol* sym);
	~PreviewDialog();

private:
	void InitLayout(wxGLContext* glctx);

	void BuildToolBar(wxSizer* top_sizer);
	void BuildEditPanel(wxSizer* top_sizer, wxGLContext* glctx);

	void OnSetLoop(wxCommandEvent& event);

private:
	PreviewPanel* m_stage;

#ifdef S2_ANIM_CURR_OLD
	s2::AnimCurr m_curr;
#else
	s2::AnimCurr2 m_curr;
#endif // S2_ANIM_CURR_OLD

}; // PreviewDialog

}

#endif // _EASYANIM_PREVIEW_DIALOG_H_
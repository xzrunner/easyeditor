#ifndef _EASYANIM_PREVIEW_DIALOG_H_
#define _EASYANIM_PREVIEW_DIALOG_H_

#include <ee/PlayControl.h>

#include <easyanim.h>

#include <wx/wx.h>

#include "PreviewSettings.h"

namespace ee { class EditPanel; }

class wxGLContext;

namespace eanim
{

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, wxGLContext* glctx);
	~PreviewDialog();

private:
	void InitLayout(wxGLContext* glctx);

	void BuildToolBar(wxSizer* top_sizer);
	void BuildEditPanel(wxSizer* top_sizer, wxGLContext* glctx);

	void OnSetCirculate(wxCommandEvent& event);
	void OnSetStop(wxCommandEvent& event);

private:
	ee::EditPanel* m_stage;

	PlaySettings m_settings;

	ee::PlayControl m_control;

}; // PreviewDialog

}

#endif // _EASYANIM_PREVIEW_DIALOG_H_
#ifndef _EASYANIM_PREVIEW_DIALOG_H_
#define _EASYANIM_PREVIEW_DIALOG_H_

#include <wx/wx.h>
#include <drag2d.h>
#include <easyanim.h>

#include "PreviewSettings.h"

namespace eanim
{

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent);
	~PreviewDialog();

private:
	void InitLayout();

	void BuildToolBar(wxSizer* top_sizer);
	void BuildEditPanel(wxSizer* top_sizer);

	void OnSetCirculate(wxCommandEvent& event);
	void OnSetStop(wxCommandEvent& event);

private:
	d2d::EditPanel* m_stage;

	PlaySettings m_settings;

	d2d::PlayControl m_control;

}; // PreviewDialog

}

#endif // _EASYANIM_PREVIEW_DIALOG_H_
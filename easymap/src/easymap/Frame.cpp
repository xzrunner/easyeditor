#include "Frame.h"
#include "SettingsDialog.h"

namespace emap
{

BEGIN_EVENT_TABLE(Frame, d2d::Frame)
	EVT_MENU(ID_SETING_EXTEND, Frame::OnExtendSetting)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: d2d::Frame(title, filetag)
{
	m_setting_menu->Append(ID_SETING_EXTEND, wxT("Extend"), wxT("Extend"));
}

void Frame::OnExtendSetting(wxCommandEvent& event)
{
	SettingDialog dlg(this);
	dlg.ShowModal();
}

}
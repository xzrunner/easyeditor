#include "Frame.h"
#include "SettingsDialog.h"
#include "PreviewDialog.h"

#include <easymap.h>

namespace emap
{

BEGIN_EVENT_TABLE(Frame, d2d::Frame)
	EVT_MENU(ID_PREVIEW, Frame::OnPreview)
	EVT_MENU(ID_SETING_EXTEND, Frame::OnExtendSetting)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: d2d::Frame(title, filetag)
{
	m_view_menu->Append(ID_PREVIEW, wxT("&Preview\tCtrl+Enter"), wxT("Play"));
	m_setting_menu->Append(ID_SETING_EXTEND, wxT("Extend"), wxT("Extend"));
}

void Frame::OnPreview(wxCommandEvent& event)
{
	SettingCfg* cfg = SettingCfg::Instance();

	std::vector<const d2d::ISprite*> sprites;
	m_task->GetAllSprite(sprites);

	PreviewDialog dlg(this, cfg->m_view_width, cfg->m_view_height, sprites);
	dlg.ShowModal();

	d2d::EditPanel* stage = const_cast<d2d::EditPanel*>(m_task->GetEditPanel());
	stage->resetCanvas();
}

void Frame::OnExtendSetting(wxCommandEvent& event)
{
	SettingDialog dlg(this);
	dlg.ShowModal();
}

}
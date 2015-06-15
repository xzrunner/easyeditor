#include "Frame.h"
#include "SettingsDialog.h"
#include "SettingCfg.h"
#include "config.h"
#include "ToolBar.h"

#include "preview/MainDialog.h"

namespace lr
{

BEGIN_EVENT_TABLE(Frame, d2d::Frame)
	EVT_MENU(ID_PREVIEW, Frame::OnPreview)
	EVT_MENU(ID_SETING_EXTEND, Frame::OnExtendSetting)
	EVT_MENU_RANGE(ID_TOOLBAR+1, ID_TOOLBAR+8, Frame::OnToolBarClick)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: d2d::Frame(title, filetag)
{
	m_view_menu->Append(ID_PREVIEW, wxT("&Preview\tCtrl+Enter"), wxT("Play"));
	m_setting_menu->Append(ID_SETING_EXTEND, wxT("Extend"), wxT("Extend"));

	m_toolbar = new ToolBar(this, ID_TOOLBAR);
}

void Frame::OnToolBarClick(wxCommandEvent& event)
{
	int idx = event.GetId() - ID_TOOLBAR - 1;
	m_toolbar->OnClick(idx);
}

void Frame::OnPreview(wxCommandEvent& event)
{
	SettingCfg* cfg = SettingCfg::Instance();

	std::vector<const d2d::ISprite*> sprites;
	m_task->getAllSprite(sprites);

	preview::MainDialog dlg(this, cfg->m_view_width * PREVIEW_SCALE, cfg->m_view_height * PREVIEW_SCALE, sprites);
	dlg.ShowModal();

	d2d::EditPanel* stage = const_cast<d2d::EditPanel*>(m_task->getEditPanel());
	stage->resetCanvas();
}

void Frame::OnExtendSetting(wxCommandEvent& event)
{
	SettingDialog dlg(this, (StagePanel*)(m_task->getEditPanel()));
	dlg.ShowModal();
}

}
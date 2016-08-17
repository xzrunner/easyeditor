#include "Frame.h"
#include "SettingsDialog.h"
#include "SettingCfg.h"
#include "config.h"
#include "ToolBar.h"
#include "Task.h"

#include "preview/MainDialog.h"
#include "view/StagePanel.h"

#include <ee/FileHelper.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/Snapshoot.h>
#include <ee/FetchAllVisitor.h>
#include <ee/StageCanvas.h>

#include <easyshape.h>

namespace lr
{

BEGIN_EVENT_TABLE(Frame, ee::Frame)
	EVT_MENU(ID_PREVIEW, Frame::OnPreview)
	EVT_MENU(ID_SETING_EXTEND, Frame::OnExtendSetting)
	EVT_MENU_RANGE(ID_TOOLBAR+1, ID_TOOLBAR+9, Frame::OnToolBarClick)
END_EVENT_TABLE()

Frame::Frame(const std::string& title, const std::string& filetag)
	: ee::Frame(title, filetag)
{
	m_view_menu->Append(ID_PREVIEW, wxT("&Preview\tCtrl+Enter"), wxT("Play"));
	m_setting_menu->Append(ID_SETING_EXTEND, wxT("Extend"), wxT("Extend"));

	m_toolbar = new ToolBar(this, ID_TOOLBAR);
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		std::string filter = GetFileFilter() + "|PNG files (*.png)|*.png";
		wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
		if (dlg.ShowModal() == wxID_OK)
		{
			std::string filename = dlg.GetPath();
			std::string ext = ee::FileHelper::GetExtension(filename);
			if (ext == "png") {
				SaveAsPNG(filename);
			} else {
				SaveAsJson(filename);
			}
		}
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::OnFullView(wxCommandEvent& event)
{
	static_cast<Task*>(m_task)->OnFullView();
}

void Frame::OnToolBarClick(wxCommandEvent& event)
{
	int idx = event.GetId() - ID_TOOLBAR - 1;
	m_toolbar->OnClick(idx);
}

void Frame::OnPreview(wxCommandEvent& event)
{
	SettingCfg* cfg = SettingCfg::Instance();

	std::vector<ee::Sprite*> sprs;
	m_task->GetAllSprite(sprs);

	wxGLContext* ctx = m_task->GetEditPanel()->GetCanvas()->GetGLContext();
	preview::MainDialog dlg(this, ctx, cfg->m_view_width * PREVIEW_SCALE, cfg->m_view_height * PREVIEW_SCALE, sprs);
	dlg.ShowModal();
}

void Frame::OnExtendSetting(wxCommandEvent& event)
{
	SettingDialog dlg(this, (StagePanel*)(m_task->GetEditPanel()));
	dlg.ShowModal();
}

void Frame::SaveAsPNG(const std::string& filepath) const
{
	SettingCfg* cfg = SettingCfg::Instance();
	ee::Snapshoot ss(cfg->m_map_width, cfg->m_map_height);
	StagePanel* stage = (StagePanel*)(m_task->GetEditPanel());

	std::vector<ee::Sprite*> sprs;
	stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs), ee::DT_VISIBLE);
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		ss.DrawSprite(sprs[i]);
	}

	std::vector<ee::Shape*> shapes;
	stage->TraverseShapes(ee::FetchAllVisitor<ee::Shape>(shapes), ee::DT_VISIBLE);
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		ss.DrawShape(shapes[i]);		
	}

	eshape::RectShape rect(sm::vec2(0, 0), cfg->m_view_width * 0.5f, cfg->m_view_height * 0.5f);
	ss.DrawShape(&rect);

	ss.SaveToFile(filepath);
}

void Frame::SaveAsJson(const std::string& filepath) const
{
	std::string fixed = ee::FileHelper::GetFilenameAddTag(filepath, m_filetag, "json");
	m_curr_filename = fixed;
	m_task->Store(fixed.c_str());
}

}
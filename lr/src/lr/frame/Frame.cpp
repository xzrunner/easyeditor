#include "Frame.h"
#include "SettingsDialog.h"
#include "SettingCfg.h"
#include "config.h"
#include "ToolBar.h"

#include "preview/MainDialog.h"
#include "view/StagePanel.h"

#include <easyshape.h>

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

void Frame::onSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		wxString filter = GetFileFilter() + "|PNG files (*.png)|*.png";
		wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
		if (dlg.ShowModal() == wxID_OK)
		{
			wxString filename = dlg.GetPath();
			wxString ext = d2d::FilenameTools::getExtension(filename);
			if (ext == "png") {
				SaveAsPNG(filename.ToStdString());
			} else {
				SaveAsJson(filename.ToStdString());
			}
		}
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
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

void Frame::SaveAsPNG(const std::string& filepath) const
{
	SettingCfg* cfg = SettingCfg::Instance();
	d2d::Snapshoot ss(cfg->m_map_width, cfg->m_map_height);
	StagePanel* stage = (StagePanel*)(m_task->getEditPanel());

	std::vector<d2d::ISprite*> sprites;
	stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites), d2d::DT_VISIBLE);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ss.DrawSprite(sprites[i]);
	}

	std::vector<d2d::IShape*> shapes;
	stage->traverseShapes(d2d::FetchAllVisitor<d2d::IShape>(shapes), d2d::DT_VISIBLE);
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		ss.DrawShape(shapes[i]);		
	}

	libshape::RectShape rect(d2d::Vector(0, 0), cfg->m_view_width * 0.5f, cfg->m_view_height * 0.5f);
	ss.DrawShape(&rect);

	ss.SaveToFile(filepath);

	stage->getCanvas()->resetInitState();
}

void Frame::SaveAsJson(const std::string& filepath) const
{
	wxString fixed = d2d::FilenameTools::getFilenameAddTag(filepath, m_filetag, "json");
	m_currFilename = fixed;
	m_task->store(fixed);
}

}
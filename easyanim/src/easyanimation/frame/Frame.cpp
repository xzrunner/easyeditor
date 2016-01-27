#include "Frame.h"
#include "CodeSettingDlg.h"
#include "Love2dCode.h"
#include "FileIO.h"
#include "frame/Task.h"
#include "view/ViewMgr.h"
#include "view/PreviewDialog.h"
#include "view/StageCanvas.h"
#include "view/StagePanel.h"

#include <easybuilder.h>

namespace eanim
{

BEGIN_EVENT_TABLE(Frame, d2d::Frame)
	EVT_MENU(ID_PREVIEW, Frame::OnPreview)
	EVT_MENU(ID_BACKGROUND, Frame::OnSetBackground)
	EVT_MENU(ID_CODESETTING, Frame::OnCodeSetting)
	EVT_MENU(ID_LOVE2D, Frame::OnCodeLove2d)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: d2d::Frame(title, filetag)
{
	m_view_menu->Append(ID_PREVIEW, wxT("&Preview\tCtrl+Enter"), wxT("Play"));
	m_setting_menu->Append(ID_BACKGROUND, wxT("Background"), wxT("Background"));
	m_code_menu->Append(ID_CODESETTING, wxT("setting..."), wxEmptyString);
	m_code_menu->Append(ID_LOVE2D, wxT("love2d"), wxEmptyString);
}

void Frame::OnOpen(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		std::string single_filter = GetJsonFileFilter(d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_anim)),
			template_filter = GetJsonFileFilter(d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_anis)),
			all_filter = "All | *_ani?.json";
		std::string filter = all_filter + "|" + single_filter + "|" + template_filter;
		wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
		if (dlg.ShowModal() == wxID_OK)
		{
			std::string filename = dlg.GetPath();
			OpenFile(filename);
		}
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
 	if (!m_task) return;
 
 	try {
		std::string single_filter = GetJsonFileFilter(d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_anim)),
			template_filter = GetJsonFileFilter(d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_anis)),
			png_filter = "PNG files (*.png)|*.png";
		std::string filter = single_filter + "|" + template_filter + "|" + png_filter;
 		wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
 		if (dlg.ShowModal() == wxID_OK)
 		{
			std::string filename = dlg.GetPath();
			int idx = dlg.GetFilterIndex();
			if (idx == 0) {
				SaveAsSingle(filename);
			} else if (idx == 1) {
				SaveAsTemplate(filename);
			} else if (idx == 2) {
				SaveAsPNG(filename);
			}
 		}
 	} catch (d2d::Exception& e) {
 		d2d::ExceptionDlg dlg(this, e);
 		dlg.ShowModal();
 	}
}

void Frame::OnPreview(wxCommandEvent& event)
{
	StagePanel* stage = ViewMgr::Instance()->stage;
	stage->EnableObserve(false);
	stage->GetCanvas()->EnableObserve(false);
	{
		PreviewDialog dlg(this, stage->GetCanvas()->GetGLContext());
		dlg.ShowModal();
	}
	stage->EnableObserve(true);
	stage->GetCanvas()->EnableObserve(true);

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void Frame::OnSetBackground(wxCommandEvent& event)
{
	std::string formatFilter("*.png;*.jpg;*.json");
	wxFileDialog dlg(this, wxT("Choose Background Symbol"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string filename = dlg.GetPath().ToStdString();
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filename);
		d2d::IStageCanvas* canvas = const_cast<d2d::EditPanel*>(m_task->GetEditPanel())->GetCanvas();
		static_cast<StageCanvas*>(canvas)->SetBackground(symbol);
		symbol->Release();
	}
	else
	{
		d2d::IStageCanvas* canvas = const_cast<d2d::EditPanel*>(m_task->GetEditPanel())->GetCanvas();
		static_cast<StageCanvas*>(canvas)->SetBackground(NULL);
	}
}

void Frame::OnCodeSetting(wxCommandEvent& event)
{
	CodeSettingDlg dlg(this);
	dlg.ShowModal();
}

void Frame::OnCodeLove2d(wxCommandEvent& event)
{
	ebuilder::CodeDialog dlg(this);

	ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, wxT("main.lua"));

	ebuilder::CodeGenerator gen;
	Love2dCode code(gen);
	code.Resolve();
	page->SetReadOnly(false);
	page->SetText(gen.toText());
	page->SetReadOnly(true);

	dlg.notebook->AddPage(page, page->getName());

	dlg.ShowModal();
}

void Frame::SaveAsPNG(const std::string& filepath) const
{
	std::vector<d2d::ISprite*> sprites;
	((StagePanel*)(m_task->GetEditPanel()))->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites), d2d::DT_VISIBLE);

	d2d::Rect region;
 	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
 		std::vector<d2d::Vector> vertices;
 		sprites[i]->GetBounding()->getBoundPos(vertices);
		for (size_t j = 0, m = vertices.size(); j < m; ++j) {
 			region.combine(vertices[j]);
		}
 	}

	d2d::Snapshoot ss(region.xLength(), region.yLength());
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		ss.DrawSprite(sprites[i], false, region.xCenter(), region.yCenter());
	}

	ss.SaveToFile(filepath);
}

void Frame::SaveAsSingle(const std::string& filepath) const
{
	std::string tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_anim);
	std::string full_path = d2d::FilenameTools::getFilenameAddTag(filepath, tag, "json");
	m_curr_filename = full_path;

	FileIO::StoreSingle(full_path);
	ViewMgr::Instance()->stage->OnSave();
}

void Frame::SaveAsTemplate(const std::string& filepath) const
{
	std::string tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_anis);
	std::string full_path = d2d::FilenameTools::getFilenameAddTag(filepath, tag, "json");
	m_curr_filename = full_path;

	FileIO::StoreTemplate(full_path);
	ViewMgr::Instance()->stage->OnSave();
}

}

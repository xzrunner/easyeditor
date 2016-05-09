#include "Frame.h"
#include "CodeSettingDlg.h"
#include "Love2dCode.h"
#include "FileIO.h"
#include "frame/Task.h"
#include "view/ViewMgr.h"
#include "view/PreviewDialog.h"
#include "view/StageCanvas.h"
#include "view/StagePanel.h"

#include <ee/FileType.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/panel_msg.h>
#include <ee/SymbolMgr.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Snapshoot.h>
#include <ee/BoundingBox.h>
#include <ee/FileHelper.h>

#include <easybuilder.h>

namespace eanim
{

BEGIN_EVENT_TABLE(Frame, ee::Frame)
	EVT_MENU(ID_PREVIEW, Frame::OnPreview)
	EVT_MENU(ID_BACKGROUND, Frame::OnSetBackground)
	EVT_MENU(ID_CODESETTING, Frame::OnCodeSetting)
	EVT_MENU(ID_LOVE2D, Frame::OnCodeLove2d)
END_EVENT_TABLE()

Frame::Frame(const std::string& title, const std::string& filetag)
	: ee::Frame(title, filetag)
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
		std::string single_filter = GetJsonFileFilter(ee::FileType::GetTag(ee::FileType::e_anim)),
			template_filter = GetJsonFileFilter(ee::FileType::GetTag(ee::FileType::e_anis)),
			all_filter = "All | *_ani?.json";
		std::string filter = all_filter + "|" + single_filter + "|" + template_filter;
		wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
		if (dlg.ShowModal() == wxID_OK)
		{
			std::string filename = dlg.GetPath();
			OpenFile(filename);
		}
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
 	if (!m_task) return;
 
 	try {
		std::string single_filter = GetJsonFileFilter(ee::FileType::GetTag(ee::FileType::e_anim)),
			template_filter = GetJsonFileFilter(ee::FileType::GetTag(ee::FileType::e_anis)),
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
 	} catch (ee::Exception& e) {
 		ee::ExceptionDlg dlg(this, e);
 		dlg.ShowModal();
 	}
}

void Frame::OnPreview(wxCommandEvent& event)
{
	StagePanel* stage = ViewMgr::Instance()->stage;
	stage->EnableObserve(false);
	stage->GetCanvas()->SetDrawable(false);
	{
		PreviewDialog dlg(this, stage->GetCanvas()->GetGLContext());
		dlg.ShowModal();
	}
	stage->EnableObserve(true);
	stage->GetCanvas()->EnableObserve(true);
	stage->GetCanvas()->SetDrawable(true);

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void Frame::OnSetBackground(wxCommandEvent& event)
{
	std::string formatFilter("*.png;*.jpg;*.json");
	wxFileDialog dlg(this, wxT("Choose Background Symbol"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string filename = dlg.GetPath().ToStdString();
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filename);
		ee::StageCanvas* canvas = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetCanvas();
		static_cast<StageCanvas*>(canvas)->SetBackground(symbol);
		symbol->Release();
	}
	else
	{
		ee::StageCanvas* canvas = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetCanvas();
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

	ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, "main.lua");

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
	std::vector<ee::Sprite*> sprites;
	((StagePanel*)(m_task->GetEditPanel()))->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites), ee::DT_VISIBLE);

	ee::Rect region;
 	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
 		std::vector<sm::vec2> vertices;
 		sprites[i]->GetBounding()->GetBoundPos(vertices);
		for (size_t j = 0, m = vertices.size(); j < m; ++j) {
 			region.Combine(vertices[j]);
		}
 	}

	ee::Snapshoot ss(region.Width(), region.Height());
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		ss.DrawSprite(sprites[i], false, region.CenterX(), region.CenterY());
	}

	ss.SaveToFile(filepath);
}

void Frame::SaveAsSingle(const std::string& filepath) const
{
	std::string tag = ee::FileType::GetTag(ee::FileType::e_anim);
	std::string full_path = ee::FileHelper::GetFilenameAddTag(filepath, tag, "json");
	m_curr_filename = full_path;

	FileIO::StoreSingle(full_path);
	ViewMgr::Instance()->stage->OnSave();
}

void Frame::SaveAsTemplate(const std::string& filepath) const
{
	std::string tag = ee::FileType::GetTag(ee::FileType::e_anis);
	std::string full_path = ee::FileHelper::GetFilenameAddTag(filepath, tag, "json");
	m_curr_filename = full_path;

	FileIO::StoreTemplate(full_path);
	ViewMgr::Instance()->stage->OnSave();
}

}

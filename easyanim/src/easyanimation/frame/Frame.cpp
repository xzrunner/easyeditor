#include "Frame.h"
#include "CodeSettingDlg.h"
#include "Love2dCode.h"
#include "FileIO.h"
#include "frame/Task.h"
#include "view/ViewMgr.h"
#include "view/PreviewDialog.h"
#include "view/StageCanvas.h"
#include "view/StagePanel.h"
#include "dataset/DataMgr.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"

#include <ee/SymbolFile.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/panel_msg.h>
#include <ee/SymbolMgr.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Snapshoot.h>
#include <ee/FileHelper.h>
#include <ee/SymbolType.h>

#include <easybuilder.h>

#include <sprite2/BoundingBox.h>
#include <sprite2/SymType.h>

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
		std::string all_filter = "All ani | *_ani?.json";
		std::string single_filter = ee::FileHelper::GetJsonFileFilter(ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION));
		std::string template_filter = ee::FileHelper::GetJsonFileFilter(ee::SymbolFile::Instance()->Tag(ee::SYM_ANIS));
		std::string json_filter = "JSON files (*.json)|*.json";
		std::string filter = all_filter + "|" + single_filter + "|" + template_filter + "|" + json_filter;
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
		std::string single_filter = ee::FileHelper::GetJsonFileFilter(ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION)),
			template_filter = ee::FileHelper::GetJsonFileFilter(ee::SymbolFile::Instance()->Tag(ee::SYM_ANIS)),
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
		s2::AnimSymbol* sym = BuildSym();
		PreviewDialog dlg(this, stage->GetCanvas()->GetGLContext(), sym);
		dlg.ShowModal();
		sym->RemoveReference();
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
		ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filename);
		ee::StageCanvas* canvas = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetCanvas();
		static_cast<StageCanvas*>(canvas)->SetBackground(sym);
		sym->RemoveReference();
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
	std::vector<ee::Sprite*> sprs;
	((StagePanel*)(m_task->GetEditPanel()))->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs), ee::DT_VISIBLE);

	sm::rect region;
 	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->GetBounding()->CombineTo(region);
 	}

	sm::vec2 sz = region.Size();
	sm::vec2 c = region.Center();
	ee::Snapshoot ss(sz.x, sz.y);
	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		ss.DrawSprite(sprs[i], false, c.x, c.y);
	}

	ss.SaveToFile(filepath);
}

void Frame::SaveAsSingle(const std::string& filepath) const
{
	std::string tag = ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION);
	std::string full_path = ee::FileHelper::GetFilenameAddTag(filepath, tag, "json");
	m_curr_filename = full_path;

	FileIO::StoreSingle(full_path);
	ViewMgr::Instance()->stage->OnSave();
}

void Frame::SaveAsTemplate(const std::string& filepath) const
{
	std::string tag = ee::SymbolFile::Instance()->Tag(ee::SYM_ANIS);
	std::string full_path = ee::FileHelper::GetFilenameAddTag(filepath, tag, "json");
	m_curr_filename = full_path;

	FileIO::StoreTemplate(full_path);
	ViewMgr::Instance()->stage->OnSave();
}

s2::AnimSymbol* Frame::BuildSym() const
{
	s2::AnimSymbol* dst = new s2::AnimSymbol();
	const std::vector<Layer*>& src = DataMgr::Instance()->GetLayers().GetAllLayers();
	for (int i = 0, n = src.size(); i < n; ++i)
	{
		const std::map<int, KeyFrame*>& src_layer = src[i]->GetAllFrames();
		s2::AnimSymbol::Layer* dst_layer = new s2::AnimSymbol::Layer;
		std::map<int, KeyFrame*>::const_iterator itr = src_layer.begin();
		for ( ; itr != src_layer.end(); ++itr)
		{
			const std::vector<ee::Sprite*>& src_frame = itr->second->GetAllSprites();
			s2::AnimSymbol::Frame* dst_frame = new s2::AnimSymbol::Frame();
			dst_frame->index = itr->second->GetTime();
			dst_frame->tween = itr->second->HasClassicTween();
			for (int j = 0, m = src_frame.size(); j < m; ++j) 
			{
				src_frame[j]->AddReference();
				dst_frame->sprs.push_back(src_frame[j]);
			}
			dst_layer->frames.push_back(dst_frame);
		}
		dst->AddLayer(dst_layer);
	}
	return dst;
}

}

#include "Frame.h"
#include "Task.h"
#include "Code.h"

#include <ee/FileHelper.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/Config.h>
#include <ee/SymbolMgr.h>
#include <ee/SettingData.h>
#include <ee/SymbolFile.h>
#include <ee/EditedFileStack.h>

#include <easycomplex.h>
#include <easycoco.h>
#include <easybuilder.h>

#include <sprite2/SymType.h>
#include <sprite2/DrawRT.h>

namespace ecomplex
{

enum MenuID
{
	ID_CODE = 700
};

BEGIN_EVENT_TABLE(Frame, ee::Frame)
	EVT_MENU(ID_PREVIEW, Frame::onPreview)
	EVT_MENU(ID_EJ_PREVIEW, Frame::OnEJPreview)
	EVT_MENU(ID_SET_BG, Frame::onSetBackground)
	EVT_MENU(ID_CODE, Frame::onCode)
END_EVENT_TABLE()

Frame::Frame(const std::string& title, const std::string& filetag)
	: ee::Frame(title, filetag)
{
	m_view_menu->Append(ID_PREVIEW, wxT("&Preview\tCtrl+Enter"), wxT("Play"));
	m_view_menu->Append(ID_EJ_PREVIEW, wxT("EJ Preview"), wxT("Preview"));
	m_setting_menu->Append(ID_SET_BG, wxT("Background"), wxT("Background"));
	m_code_menu->Append(ID_CODE, wxT("Info\tCtrl+I"), wxT("Info"));

	SetIcon(wxICON(aaaa));
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		std::string filter = GetFileFilter();
		std::string png_filter = "PNG files (*.png)|*.png";
		filter += "|" + png_filter;
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

void Frame::OnSettings(wxCommandEvent& event)
{
	SettingsDialog dlg(this);
	dlg.ShowModal();

	const s2::Color& col = ee::Config::Instance()->GetSettings().bg_color;
	const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetCanvas()->SetBgColor(col);
}

std::string Frame::GetFileFilter() const
{
	std::string tag = ee::SymbolFile::Instance()->Tag(s2::SYM_COMPLEX);
	std::string complex_filter = ee::FileHelper::GetJsonFileFilter(tag),
		        psd_filter = ee::FileHelper::GetJsonFileFilter("psd");
	return complex_filter + "|" + psd_filter;
}

void Frame::onPreview(wxCommandEvent& event)
{
// 	ee::Symbol* sym = ee::SymbolMgr::Instance()->fetchSymbol("default.ttf");
// 	sym->ReloadTexture();

	std::vector<ee::Sprite*> sprs;
	m_task->GetAllSprite(sprs);
	ee::StageCanvas* canvas = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetCanvas();
 	PreviewDialog dlg(this, canvas->GetGLContext(), sprs);
 	dlg.ShowModal();
}

void Frame::OnEJPreview(wxCommandEvent& event)
{
	std::vector<ee::Sprite*> sprs;
	m_task->GetAllSprite(sprs);

	//////////////////////////////////////////////////////////////////////////

	// 		EJPreviewDialog dlg(this, sprs);
	// 		dlg.ShowModal();

	//////////////////////////////////////////////////////////////////////////

	const char* folder = "_tmp_ejoy2d_preview";
	ee::FileHelper::MkDir(folder);

	ecoco::epe::PackLuaFile pack;
	pack.pack(sprs, folder);

#ifdef _DEBUG
	std::string cwd = wxFileName::GetCwd();
	std::string workpath = cwd + "\\..\\..\\..\\..\\..\\editor_bin\\";
	std::string cmd = workpath + "ejoy2d.exe " + workpath + "ejoy2d\\preview\\play.lua";
#else
	std::string cmd = "ejoy2d.exe ejoy2d/preview/play.lua";
#endif
	WinExec(cmd.c_str(), /*SW_SHOWMAXIMIZED*/SW_NORMAL);
}

void Frame::onSetBackground(wxCommandEvent& event)
{
	std::string formatFilter = "*.png;*.jpg;*.json";
	wxFileDialog dlg(this, wxT("Choose Background Symbol"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN);
	ee::StageCanvas* canvas = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetCanvas();
	if (dlg.ShowModal() == wxID_OK)
	{
 		std::string filename = dlg.GetPath().ToStdString();
		ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filename);
  		static_cast<StageCanvas*>(canvas)->SetBackground(sym);
		sym->RemoveReference();
	}
	else
	{
		static_cast<StageCanvas*>(canvas)->SetBackground(NULL);
	}
}

void Frame::onCode(wxCommandEvent& event)
{
	ebuilder::CodeDialog dlg(this);
	StagePanel* stage = static_cast<StagePanel*>(
		const_cast<ee::EditPanel*>(m_task->GetEditPanel()));
	// ui
	{
		ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, "ui.lua");

		ebuilder::CodeGenerator gen;
		Code code(gen);
		code.ResolveUI(*stage->GetSymbol());
		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);

		dlg.notebook->AddPage(page, page->getName());
	}
	// tid
	{
		ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, "texts.lua");

		ebuilder::CodeGenerator gen;
		Code code(gen);
		code.ResolveText(*stage->GetSymbol());
		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);

		dlg.notebook->AddPage(page, page->getName());
	}
	dlg.ShowModal();

	//////////////////////////////////////////////////////////////////////////

// 	Json::Value value;
// 	Json::Reader reader;
// 	std::locale::global(std::locale(""));
// 	std::ifstream fin("E:\\debug\\polygon\\debug.json");
// 	std::locale::global(std::locale("C"));
// 	reader.parse(fin, value);
// 	fin.close();
// 
// 	std::vector<sm::vec2> outline, segments;
// 	ee::JsonIO::Load(value["outline"], outline);
// 	ee::JsonIO::Load(value["segments"], segments);
// 
// 	std::vector<sm::vec2> tris;
// 	sm::triangulate_lines(outline, segments, tris);
// 	
// 	int zz = 0;
}

void Frame::SaveAsPNG(const std::string& filepath) const
{
	Symbol* sym = ((StagePanel*)(m_task->GetEditPanel()))->GetSymbol();
	sm::vec2 sz = sym->GetBounding().Size();
	s2::DrawRT rt(sz.x, sz.y);
	rt.Draw(sym);
	rt.StoreToFile(filepath);
}

void Frame::SaveAsJson(const std::string& filepath) const
{
	std::string fixed = ee::FileHelper::GetFilenameAddTag(filepath, m_filetag, "json");
	ee::EditedFileStack::Instance()->SetBottom(fixed);
	m_task->Store(fixed.c_str());
}

}
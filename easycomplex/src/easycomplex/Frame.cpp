#include "Frame.h"
#include "Task.h"

#include <easycomplex.h>
#include <easycoco.h>

namespace ecomplex
{

BEGIN_EVENT_TABLE(Frame, d2d::Frame)
	EVT_MENU(ID_PREVIEW, Frame::onPreview)
	EVT_MENU(ID_EJ_PREVIEW, Frame::onEJPreview)
	EVT_MENU(ID_SET_BG, Frame::onSetBackground)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: d2d::Frame(title, filetag)
{
	m_view_menu->Append(ID_PREVIEW, wxT("&Preview\tCtrl+Enter"), wxT("Play"));
	m_view_menu->Append(ID_EJ_PREVIEW, wxT("EJ Preview"), wxT("Preview"));
	m_setting_menu->Append(ID_SET_BG, wxT("Background"), wxT("Background"));

#ifdef _DEBUG
	wxLog::SetActiveTarget(new wxLogWindow(this, _T("Log window")));
	m_log_chain = new wxLogChain(new wxLogStderr);
#endif
}

Frame::~Frame()
{
#ifdef _DEBUG
	delete m_log_chain;
#endif
}

void Frame::onSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		wxString filter = "JSON files (*.json)|*.json|PNG files (*.png)|*.png";
		wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
		if (dlg.ShowModal() == wxID_OK)
		{
			wxString filename = dlg.GetPath();
			wxString ext = d2d::FilenameTools::getExtension(filename);
			if (ext == "png")
			{
				d2d::Snapshoot ss;
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(m_currFilename);
				ss.outputToImageFile(symbol, filename.ToStdString());
				symbol->release();
			}
			else
			{
				wxString fixed = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), m_filetag, "json");
				m_currFilename = fixed;
				m_task->store(fixed);
			}
		}
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::onSettings(wxCommandEvent& event)
{
	SettingsDialog dlg(this);
	dlg.ShowModal();
}

void Frame::onPreview(wxCommandEvent& event)
{
	int old_font_type = d2d::Settings::DrawFontType;
	d2d::Settings::DrawFontType = d2d::Settings::DrawFontText;

	bool old_name_type = d2d::Settings::bVisibleNodeName;
	d2d::Settings::bVisibleNodeName = true;

	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol("default.ttf");
	symbol->reloadTexture();

	std::vector<const d2d::ISprite*> sprites;
	m_task->getAllSprite(sprites);
 	PreviewDialog dlg(this, sprites);
 	dlg.ShowModal();

	d2d::EditPanel* stage = const_cast<d2d::EditPanel*>(m_task->getEditPanel());
 	stage->resetCanvas();

	d2d::Settings::DrawFontType = old_font_type;
	d2d::Settings::bVisibleNodeName = old_name_type;
}

void Frame::onEJPreview(wxCommandEvent& event)
{
	std::vector<const d2d::ISprite*> sprites;
	m_task->getAllSprite(sprites);

	//////////////////////////////////////////////////////////////////////////

	// 		EJPreviewDialog dlg(this, sprites);
	// 		dlg.ShowModal();

	//////////////////////////////////////////////////////////////////////////

	const char* folder = "_tmp_ejoy2d_preview";
	d2d::MkDirRF(folder);

	libcoco::PackLuaFile pack;
	pack.pack(sprites, folder);

#ifdef _DEBUG
	wxString cwd = wxFileName::GetCwd();
	std::string workpath = cwd + "\\..\\..\\..\\..\\..\\editor_bin\\";
	std::string cmd = workpath + "ejoy2d.exe " + workpath + "ejoy2d\\preview\\play.lua";
#else
	std::string cmd = "ejoy2d.exe ejoy2d/preview/play.lua";
#endif
	WinExec(cmd.c_str(), /*SW_SHOWMAXIMIZED*/SW_NORMAL);
}

void Frame::onSetBackground(wxCommandEvent& event)
{
	wxString formatFilter = wxT("*.png;*.jpg");
	wxFileDialog dlg(this, wxT("Choose Background Image"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
 		wxString filename = dlg.GetPath();
 		d2d::Image* img = d2d::ImageMgr::Instance()->getItem(filename);
		StagePanel* stage = static_cast<Task*>(m_task)->getStagePanel();
 		d2d::GLCanvas* canvas = stage->getCanvas();
 		static_cast<StageCanvas*>(canvas)->setBackground(img);
		img->release();
	}
}

}
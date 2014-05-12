#include "Frame.h"
#include "Task.h"

#include <easycomplex.h>
#include <easycoco.h>

namespace ecomplex
{

BEGIN_EVENT_TABLE(Frame, d2d::Frame)
	EVT_MENU(ID_EJ_PREVIEW, Frame::onEJPreview)
	EVT_MENU(ID_SET_BG, Frame::onSetBackground)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: d2d::Frame(title, filetag)
{
	m_view_menu->Append(ID_EJ_PREVIEW, wxT("EJ Preview"), wxT("Preview"));
	m_setting_menu->Append(ID_SET_BG, wxT("Background"), wxT("Background"));
}

void Frame::onSettings(wxCommandEvent& event)
{
	SettingsDialog dlg(this);
	dlg.ShowModal();
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
	}
}

}
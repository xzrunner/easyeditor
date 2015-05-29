#include "Frame.h"

#include "CodeSettingDlg.h"
#include "Love2dCode.h"

#include "frame/Task.h"
#include "view/PreviewDialog.h"
#include "view/StageCanvas.h"

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

void Frame::OnPreview(wxCommandEvent& event)
{
	PreviewDialog dlg(this, static_cast<Task*>(m_task)->GetController());
	dlg.ShowModal();

	const_cast<d2d::EditPanel*>(m_task->getEditPanel())->ResetViewport();
}

void Frame::OnSetBackground(wxCommandEvent& event)
{
	wxString formatFilter = wxT("*.png;*.jpg");
	wxFileDialog dlg(this, wxT("Choose Background Image"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString filename = dlg.GetPath();
		d2d::Image* img = d2d::ImageMgr::Instance()->getItem(filename);
		d2d::GLCanvas* canvas = const_cast<d2d::EditPanel*>(m_task->getEditPanel())->getCanvas();
		static_cast<StageCanvas*>(canvas)->setBackground(img);
	}
}

void Frame::OnCodeSetting(wxCommandEvent& event)
{
	CodeSettingDlg dlg(this, static_cast<Task*>(m_task)->GetController());
	dlg.ShowModal();
}

void Frame::OnCodeLove2d(wxCommandEvent& event)
{
	ebuilder::CodeDialog dlg(this);

	ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, wxT("main.lua"));

	ebuilder::CodeGenerator gen;
	Love2dCode code(gen, static_cast<Task*>(m_task)->GetController());
	code.resolve();
	page->SetReadOnly(false);
	page->SetText(gen.toText());
	page->SetReadOnly(true);

	dlg.notebook->AddPage(page, page->getName());

	dlg.ShowModal();
}

void Frame::SaveAsPNG(const std::string& filepath) const
{
	d2d::Snapshoot ss;
	libanim::Symbol* symbol = ((StagePanel*)(m_task->getEditPanel()))->getSymbol();
	symbol->InitBounding();
	ss.outputToImageFile(symbol, filepath);
	m_task->getEditPanel()->getCanvas()->resetInitState();
}

void Frame::SaveAsJson(const std::string& filepath) const
{
	wxString fixed = d2d::FilenameTools::getFilenameAddTag(filepath, m_filetag, "json");
	m_currFilename = fixed;
	m_task->store(fixed);
}

}

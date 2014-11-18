#include "Frame.h"
#include "Task.h"

#include <easymesh.h>

namespace emesh
{

BEGIN_EVENT_TABLE(Frame, d2d::Frame)
	EVT_MENU(ID_SET_BG, Frame::OnSetBackground)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: d2d::Frame(title, filetag)
{
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

void Frame::OnSetBackground(wxCommandEvent& event)
{
	wxString formatFilter = wxT("*.png;*.jpg");
	wxFileDialog dlg(this, wxT("Choose Background Image"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString filename = dlg.GetPath();

		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filename);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		StagePanel* stage = static_cast<Task*>(m_task)->getStagePanel();
		stage->SetBackground(sprite);
		sprite->release();
		symbol->release();
		stage->Refresh();
	}
}

}
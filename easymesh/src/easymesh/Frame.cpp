#include "Frame.h"
#include "Task.h"

#include <easymesh.h>

namespace emesh
{

BEGIN_EVENT_TABLE(Frame, ee::Frame)
	EVT_MENU(ID_SET_BG, Frame::OnSetBackground)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: ee::Frame(title, filetag)
{
	m_setting_menu->Append(ID_SET_BG, wxT("Background"), wxT("Background"));
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		wxString filter = "JSON files (*.json)|*.json|PNG files (*.png)|*.png";
		wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
		if (dlg.ShowModal() == wxID_OK)
		{
			wxString filename = dlg.GetPath();
			wxString ext = ee::FileHelper::GetExtension(filename);
			if (ext == "png")
			{
				ee::Snapshoot ss;
				ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(m_curr_filename);
				ss.OutputToImageFile(symbol, filename.ToStdString());
				symbol->Release();
			}
			else
			{
				wxString fixed = ee::FileHelper::GetFilenameAddTag(dlg.GetPath(), m_filetag, "json");
				m_curr_filename = fixed;
				m_task->Store(fixed);
			}
		}
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(this, e);
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
		std::string filename = dlg.GetPath().ToStdString();

		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filename);
		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		StagePanel* stage = static_cast<Task*>(m_task)->getStagePanel();
		stage->SetBackground(sprite);
		sprite->Release();
		symbol->Release();
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

}
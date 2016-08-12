#include "Frame.h"
#include "Task.h"

#include <easymesh.h>

#include <ee/FileHelper.h>
#include <ee/Snapshoot.h>
#include <ee/SymbolMgr.h>
#include <ee/ExceptionDlg.h>
#include <ee/Exception.h>
#include <ee/SpriteFactory.h>
#include <ee/panel_msg.h>

namespace emesh
{

BEGIN_EVENT_TABLE(Frame, ee::Frame)
	EVT_MENU(ID_SET_BG, Frame::OnSetBackground)
END_EVENT_TABLE()

Frame::Frame(const std::string& title, const std::string& filetag)
	: ee::Frame(title, filetag)
{
	m_setting_menu->Append(ID_SET_BG, wxT("Background"), wxT("Background"));
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		std::string filter = "JSON files (*.json)|*.json|PNG files (*.png)|*.png";
		wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
		if (dlg.ShowModal() == wxID_OK)
		{
			std::string filename = dlg.GetPath();
			std::string ext = ee::FileHelper::GetExtension(filename);
			if (ext == "png")
			{
				ee::Snapshoot ss;
				ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(m_curr_filename);
				ss.OutputToImageFile(symbol, filename);
				symbol->RemoveReference();
			}
			else
			{
				std::string fixed = ee::FileHelper::GetFilenameAddTag(dlg.GetPath().ToStdString(), m_filetag, "json");
				m_curr_filename = fixed;
				m_task->Store(fixed.c_str());
			}
		}
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::OnSetBackground(wxCommandEvent& event)
{
	std::string filter = "*.png;*.jpg";
	wxFileDialog dlg(this, wxT("Choose Background Image"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string filename = dlg.GetPath().ToStdString();

		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filename);
		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		StagePanel* stage = static_cast<Task*>(m_task)->getStagePanel();
		stage->SetBackground(sprite);
		sprite->RemoveReference();
		symbol->RemoveReference();
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

}
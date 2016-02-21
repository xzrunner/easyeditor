#include "Frame.h"
#include "StagePanel.h"
#include "StageCanvas.h"

#include <ee/Task.h>
#include <ee/SymbolMgr.h>
#include <ee/panel_msg.h>
#include <ee/Symbol.h>

namespace sg
{

BEGIN_EVENT_TABLE(Frame, ee::Frame)
	EVT_MENU(ID_CHANGE_PERSPECTIVE, Frame::onChangePerspective)
	EVT_MENU(ID_SET_BG, Frame::onSetBackground)
END_EVENT_TABLE()

Frame::Frame(const std::string& title, const std::string& filetag)
	: ee::Frame(title, filetag)
{
	m_view_menu->Append(ID_CHANGE_PERSPECTIVE, wxT("Perspective"), wxT("Perspective"));
	m_setting_menu->Append(ID_SET_BG, wxT("Background"), wxT("Background"));
}

void Frame::onChangePerspective(wxCommandEvent& event)
{
	const StagePanel* stage = static_cast<const StagePanel*>((m_task)->GetEditPanel());
	const_cast<StagePanel*>(stage)->SetPerspective(!stage->GetPerspective());
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void Frame::onSetBackground(wxCommandEvent& event)
{
	std::string filter = "*.png;*.jpg;*.json";
	wxFileDialog dlg(this, wxT("Choose Background"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string filename = dlg.GetPath().ToStdString();
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filename);
		ee::StageCanvas* canvas = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetCanvas();
		static_cast<StageCanvas*>(canvas)->SetBackground(symbol);
		symbol->Release();
	}
}

}
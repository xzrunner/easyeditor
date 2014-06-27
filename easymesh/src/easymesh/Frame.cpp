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
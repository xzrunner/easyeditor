#include "Frame.h"
#include "Task.h"

#include <easycomplex.h>

namespace ecomplex
{

BEGIN_EVENT_TABLE(Frame, d2d::Frame)
	EVT_MENU(ID_SET_BG, Frame::onSetBackground)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: d2d::Frame(title, filetag)
{
	m_setting_menu->Append(ID_SET_BG, wxT("Background", wxT("Background")));
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
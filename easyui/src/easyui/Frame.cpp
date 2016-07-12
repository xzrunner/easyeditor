#include "Frame.h"
#include "Task.h"
#include "UIStagePage.h"

namespace eui
{

BEGIN_EVENT_TABLE(Frame, ee::Frame)
	EVT_MENU(ID_PREVIEW, Frame::OnPreview)
	EVT_MENU(ID_CODE, Frame::OnCode)
END_EVENT_TABLE()

Frame::Frame(const std::string& title, const std::string& filetag)
	: ee::Frame(title, filetag)
{
	m_view_menu->Append(ID_PREVIEW, wxT("&Preview\tCtrl+Enter"), wxT("Play"));
	m_code_menu->Append(ID_CODE, wxT("Info\tCtrl+I"), wxT("Info"));
}

void Frame::SetFileFilter(const std::string& filter)
{
	m_filetag = filter;
}

void Frame::OnPreview(wxCommandEvent& event)
{
	static_cast<Task*>(m_task)->GetSelectedStagePage()->OnPreview();
}

void Frame::OnCode(wxCommandEvent& event)
{
	static_cast<Task*>(m_task)->GetSelectedStagePage()->OnCode();
}

}
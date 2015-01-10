#include "Frame.h"

namespace eterrain3d
{

Frame::Frame(const wxString& title)
	: d2d::Frame(title, "")
{
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

void Frame::onNew(wxCommandEvent& event)
{
}

void Frame::onOpen(wxCommandEvent& event)
{
}

void Frame::onSave(wxCommandEvent& event)
{
}

void Frame::onSaveAs(wxCommandEvent& event)
{
}

}
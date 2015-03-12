#ifdef EGUI_WX

#include "interface/Frame.h"

#include <wx/frame.h>

namespace egui
{

//////////////////////////////////////////////////////////////////////////
// class FrameImpl
//////////////////////////////////////////////////////////////////////////

class FrameImpl : public wxFrame
{
public:
	FrameImpl(const std::string& title);

private:
	void OnClose(wxCloseEvent& event);

private:
	DECLARE_EVENT_TABLE()

}; // FrameImpl

BEGIN_EVENT_TABLE(FrameImpl, wxFrame)
	EVT_CLOSE(FrameImpl::OnClose)
END_EVENT_TABLE()

FrameImpl::FrameImpl(const std::string& title)
	: wxFrame(NULL, wxID_ANY, title)
{
}

void FrameImpl::OnClose(wxCloseEvent& event)
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
// class Frame
//////////////////////////////////////////////////////////////////////////

Frame::Frame(const std::string& title)
{
	m_impl = new FrameImpl(title);
}

Frame::~Frame()
{
	delete m_impl;
}

void* Frame::GetRawPtr()
{
	return m_impl;
}

void Frame::Show()
{
	m_impl->Show(true);
}

}

#endif // EGUI_WX
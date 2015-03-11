#include "FrameWX.h"

namespace egui
{

FrameWX::FrameWX(const std::string& title)
{
	m_impl = new Impl(title);
}

FrameWX::~FrameWX()
{
	delete m_impl;
}

void* FrameWX::GetRawPtr()
{
	return m_impl;
}

void FrameWX::Show()
{
	m_impl->Show(true);
}

//////////////////////////////////////////////////////////////////////////
// class FrameWX::Impl
//////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(FrameWX::Impl, wxFrame)
	EVT_CLOSE(FrameWX::Impl::OnClose)
END_EVENT_TABLE()

FrameWX::Impl::
Impl(const std::string& title)
	: wxFrame(NULL, wxID_ANY, title)
{
}

void FrameWX::Impl::
OnClose(wxCloseEvent& event)
{
	Destroy();
}


}

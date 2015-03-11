#include "DialogWX.h"

#include "interface/Window.h"

namespace egui
{

DialogWX::DialogWX(Window* parent, const std::string& title)
{
	wxWindow* raw_wnd = static_cast<wxWindow*>(parent->GetRawPtr());
	m_impl = new wxDialog(raw_wnd, wxID_ANY, title);
}

DialogWX::~DialogWX()
{
	delete m_impl;
}

void DialogWX::ShowModal()
{
	m_impl->ShowModal();
}

}
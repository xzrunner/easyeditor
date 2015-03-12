#ifdef EGUI_WX

#include "interface/Panel.h"

#include <wx/panel.h>

namespace egui
{

//////////////////////////////////////////////////////////////////////////
// class PanelImpl
//////////////////////////////////////////////////////////////////////////

class PanelImpl : public wxPanel
{
public:
	PanelImpl(wxWindow* parent, Panel* panel, const wxSize& size);

private:
	void OnSize(wxSizeEvent& event);

private:
	Panel* m_panel;

}; // PanelImpl

PanelImpl::PanelImpl(wxWindow* parent, Panel* panel, const wxSize& size)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, size)
	, m_panel(panel)
{
}

void PanelImpl::OnSize(wxSizeEvent& event)
{
	wxSize sz = event.GetSize();
	m_panel->OnSize(sz.x, sz.y);
}

//////////////////////////////////////////////////////////////////////////
// class Panel
//////////////////////////////////////////////////////////////////////////

Panel::Panel(Window* parent, int w, int h)
{
	wxWindow* raw_wnd = static_cast<wxWindow*>(parent->GetRawPtr());
	m_impl = new PanelImpl(raw_wnd, this, wxSize(w, h));
}

Panel::~Panel()
{
	delete m_impl;
}

void Panel::OnSize(int w, int h)
{
}

}

#endif // EGUI_WX
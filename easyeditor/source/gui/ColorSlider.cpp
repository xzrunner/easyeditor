#include "ColorSlider.h"
#include "ColorMonitor.h"
#include "RGBColorPanel.h"
#include "HSLColorPanel.h"

#include <wx/spinctrl.h>

namespace ee
{

ColorSlider::ColorSlider(wxWindow* parent, 
						 ColorMonitor* color_monitor,
						 const std::string& title,
						 bool is_rgb)
	: wxPanel(parent, wxID_ANY)
	, m_color_monitor(color_monitor)
{
	InitLayout(title, is_rgb);
}

int ColorSlider::GetColorValue() const
{
	return m_slider->GetValue();
}

void ColorSlider::SetColorValue(int col)
{
	m_slider->SetValue(col);
	m_spin->SetValue(col);
}

void ColorSlider::SetColorRegion(const s2::Color& begin, const s2::Color& end)
{
	m_color_bg->SetColor(begin, end);
}

void ColorSlider::InitLayout(const std::string& title, bool is_rgb)
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	// color
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		m_title = new wxStaticText(this, wxID_ANY, title, wxDefaultPosition, wxSize(70, 20));
		sizer->Add(m_title);

		wxPanel* panel;
		if (is_rgb) {
			RGBColorPanel* cp = new RGBColorPanel(this, wxSize(350, 20));
			panel = cp;
			m_color_bg = cp;
		} else {
			HSLColorPanel* cp = new HSLColorPanel(this, wxSize(350, 20));
			panel = cp;
			m_color_bg = cp;
		}
		sizer->Add(panel);

		top_sizer->Add(sizer);
	}
	// controller
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		m_spin = new wxSpinCtrl(this, wxID_ANY, "255", wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS | wxALIGN_RIGHT | wxTE_PROCESS_ENTER, 0, 255);
		Connect(m_spin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ColorSlider::OnSetValue));
		sizer->Add(m_spin);

		m_slider = new wxSlider(this, wxID_ANY, 128, 0, 255, wxDefaultPosition, wxSize(370, -1));
		Connect(m_slider->GetId(), wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(ColorSlider::OnSetValue));
		sizer->Add(m_slider);

		top_sizer->Add(sizer);
	}
	SetSizer(top_sizer);
	top_sizer->Fit(this);
}

void ColorSlider::OnSetValue(wxScrollEvent& event)
{
	int val = m_slider->GetValue();
	m_spin->SetValue(val);
	m_color_monitor->OnColorChanged();
}

void ColorSlider::OnSetValue(wxSpinEvent& event)
{
	int val = m_spin->GetValue();
	m_slider->SetValue(val);
	m_color_monitor->OnColorChanged();
}

}
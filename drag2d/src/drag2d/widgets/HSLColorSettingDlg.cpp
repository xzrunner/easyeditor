#include "HSLColorSettingDlg.h"
#include "ColorSlider.h"

namespace d2d
{

HSLColorSettingDlg::HSLColorSettingDlg(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, "HSL Color Setting", wxDefaultPosition, wxSize(450, 300))
	, m_hue(NULL)
	, m_saturation(NULL)
	, m_lightness(NULL)
{
	InitLayout();
}

void HSLColorSettingDlg::OnColorChanged()
{
}

void HSLColorSettingDlg::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	// color
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "color");
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		m_color_bg = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 100));
		sizer->Add(m_color_bg);
		top_sizer->Add(sizer);
	}
	// setting
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "setting");
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_hue = new ColorSlider(this, this, "色相");
		sizer->Add(m_hue);
		sizer->AddSpacer(10);
		m_saturation = new ColorSlider(this, this, "饱和度");
		sizer->Add(m_saturation);
		sizer->AddSpacer(10);
		m_lightness = new ColorSlider(this, this, "亮度");
		sizer->Add(m_lightness);

		top_sizer->Add(sizer);
	}	
	SetSizer(top_sizer);
	top_sizer->Fit(this);
}

}
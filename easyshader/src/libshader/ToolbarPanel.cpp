#include "ToolBarPanel.h"
#include "SliderCtrl.h"

namespace eshader
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::EditPanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
}

void ToolbarPanel::AddUniform(const std::string& title, const std::string& name, 
							  Uniform* uniform, 
							  const std::vector<SliderItemFloat>& items)
{
 	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
 	SliderCtrl* slider = new SliderCtrl(this, title, name, uniform, items, 0.01f);
 	sizer->Add(slider);
	SetSizer(sizer);
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	return sizer;
}

}
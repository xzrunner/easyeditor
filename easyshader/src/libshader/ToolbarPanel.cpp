#include "ToolBarPanel.h"
#include "SliderCtrl.h"

namespace eshader
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::EditPanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	SetSizer(new wxBoxSizer(wxVERTICAL));
}

ToolbarPanel::~ToolbarPanel()
{
	Clear();
}

void ToolbarPanel::AddUniform(const std::string& title, const std::string& name, 
							  Uniform* uniform, 
							  const std::vector<SliderItemFloat>& items)
{
 	wxSizer* sizer = GetSizer();
 	SliderCtrl* slider = new SliderCtrl(this, title, name, uniform, items, 0.01f, 
		m_editPanel->getCanvas());
 	sizer->Add(slider);
	SetSizer(sizer);

	m_sliders.push_back(slider);

	SetSizer(sizer);

	Layout();
}

wxSizer* ToolbarPanel::initLayout()
{
	return NULL;
}

void ToolbarPanel::Clear()
{
	for_each(m_sliders.begin(), m_sliders.end(), DeletePointerFunctor<SliderCtrl>());
	m_sliders.clear();

	GetSizer()->Clear();
}

}
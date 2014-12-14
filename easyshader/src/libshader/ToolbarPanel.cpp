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
							  Shader* shader, Uniform* uniform, 
							  const std::vector<SliderItemInt>& items)
{
	if (!items.empty()) {
		AddSlider(new SliderCtrl(this, title, name, shader, uniform, 
			items, 1, m_editPanel->getCanvas()));
	}
}

void ToolbarPanel::AddUniform(const std::string& title, const std::string& name, 
							  Shader* shader, Uniform* uniform, 
							  const std::vector<SliderItemFloat>& items)
{
	if (!items.empty()) {
		AddSlider(new SliderCtrl(this, title, name, shader, uniform, 
			items, 0.01f, m_editPanel->getCanvas()));
	}
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

void ToolbarPanel::AddSlider(SliderCtrl* slider)
{
	wxSizer* sizer = GetSizer();
	sizer->Add(slider);
	SetSizer(sizer);

	m_sliders.push_back(slider);

	SetSizer(sizer);

	Layout();
}

}
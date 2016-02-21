#include "ToolBarPanel.h"
#include "SliderCtrl.h"

#include <ee/EditPanel.h>
#include <ee/EditPanelImpl.h>
#include <ee/std_functor.h>

#include <algorithm>

namespace eshader
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, ee::EditPanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
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
			items, 1, m_stage->GetCanvas()));
	}
}

void ToolbarPanel::AddUniform(const std::string& title, const std::string& name, 
							  Shader* shader, Uniform* uniform, 
							  const std::vector<SliderItemFloat>& items)
{
	if (!items.empty()) {
		AddSlider(new SliderCtrl(this, title, name, shader, uniform, 
			items, 0.01f, m_stage->GetCanvas()));
	}
}

wxSizer* ToolbarPanel::InitLayout()
{
	return NULL;
}

void ToolbarPanel::Clear()
{
	for_each(m_sliders.begin(), m_sliders.end(), ee::DeletePointerFunctor<SliderCtrl>());
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
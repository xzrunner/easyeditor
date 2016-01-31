#include "LayersMgrWidget.h"
#include "Layer.h"
#include "std_functor.h"
#include "panel_msg.h"

#include <algorithm>

namespace ee
{

LayersMgrWidget::LayersMgrWidget(wxWindow* parent)
	: wxPanel(parent)
	, m_edited_layer(NULL)
	, m_layers_sz(NULL)
	, m_btn_add(NULL), m_btn_del(NULL)
	, m_btn_up(NULL), m_btn_down(NULL)
{
	InitLayout();
	OnAddLayer(wxCommandEvent());

	m_btn_del->Enable(false);
	m_btn_down->Enable(false);
	m_btn_up->Enable(false);
}

LayersMgrWidget::~LayersMgrWidget()
{
	for_each(m_layers.begin(), m_layers.end(), DeletePointerFunctor<LayerWidget>());
	m_layers.clear();
	m_edited_layer = NULL;
}

void LayersMgrWidget::TraverseEditableLayers(Visitor& visitor)
{
	if (m_edited_layer)
	{
		bool next;
		visitor.Visit(m_edited_layer, next);
	}
}

void LayersMgrWidget::TraverseVisibleLayers(Visitor& visitor)
{
	std::vector<LayerWidget*>::const_iterator itr = m_layers.begin();
	for ( ; itr != m_layers.end(); ++itr)
	{
		if ((*itr)->IsVisible())
		{
			bool next;
			visitor.Visit((*itr)->GetLayer(), next);
			if (!next) break;
		}
	}
}

void LayersMgrWidget::TraverseSelectableLayers(Visitor& visitor)
{
	std::vector<LayerWidget*>::const_iterator itr = m_layers.begin();
	for ( ; itr != m_layers.end(); ++itr)
	{
		if ((*itr)->IsSelectable())
		{
			bool next;
			visitor.Visit((*itr)->GetLayer(), next);
			if (!next) break;
		}
	}
}

void LayersMgrWidget::TraverseAllLayers(Visitor& visitor)
{
	std::vector<LayerWidget*>::const_iterator itr = m_layers.begin();
	for ( ; itr != m_layers.end(); ++itr)
	{
		bool next;
		visitor.Visit((*itr)->GetLayer(), next);
		if (!next) break;
	}
}

void LayersMgrWidget::SetEditable(LayerWidget* layer)
{
	m_edited_layer = layer->GetLayer();
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
		m_layers[i]->SetEditable(m_layers[i] == layer);

	if (layer == m_layers.front() && layer == m_layers.back())
	{
		m_btn_down->Enable(false);
		m_btn_up->Enable(false);
	}
	else if (layer == m_layers.front())
	{
		m_btn_down->Enable(false);
		m_btn_up->Enable(true);
	}
	else if (layer == m_layers.back())
	{
		m_btn_down->Enable(true);
		m_btn_up->Enable(false);
	}
	else
	{
		m_btn_down->Enable(true);
		m_btn_up->Enable(true);
	}
}

void LayersMgrWidget::GetAllLayers(std::vector<Layer*>& layers) const
{
	layers.reserve(m_layers.size());
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
		layers.push_back(m_layers[i]->GetLayer());
}

void LayersMgrWidget::AddLayer(Layer* layer)
{
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
		m_layers[i]->SetEditable(false);
	
	LayerWidget* wrapper = new LayerWidget(this, layer);
	m_edited_layer = wrapper->GetLayer();
	m_layers_sz->Insert(0, wrapper);
	m_layers.push_back(wrapper);

	m_btn_del->Enable(true);
	m_btn_down->Enable(true);
	m_btn_up->Enable(false);

	m_parent->Fit();
}

void LayersMgrWidget::AddLayerBottom(Layer* layer)
{
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
		m_layers[i]->SetEditable(false);

	LayerWidget* wrapper = new LayerWidget(this, layer);
	m_edited_layer = wrapper->GetLayer();
	m_layers_sz->Insert(m_layers.size(), wrapper);
	m_layers.insert(m_layers.begin(), wrapper);

	m_btn_del->Enable(true);
	m_btn_down->Enable(true);
	m_btn_up->Enable(false);

	m_parent->Fit();
}

void LayersMgrWidget::Clear()
{
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
		m_layers_sz->Detach(0);

	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		delete m_layers[i]->GetLayer();
		delete m_layers[i];
	}
	m_layers.clear();

	m_edited_layer = NULL;

	m_btn_del->Enable(false);
	m_btn_up->Enable(false);
	m_btn_down->Enable(false);

	m_parent->GetParent()->Layout();
}

void LayersMgrWidget::InitLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Layers"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	topSizer->AddSpacer(5);
	topSizer->Add(InitButtonsPanel());
	topSizer->AddSpacer(2);
	topSizer->Add(m_layers_sz = InitLayersPanel());

	SetSizer(topSizer);
}

wxSizer* LayersMgrWidget::InitLayersPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxEmptyString);
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	return topSizer;
}

wxSizer* LayersMgrWidget::InitButtonsPanel()
{
	wxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	{
		m_btn_add = new wxButton(this, wxID_ANY, wxT("+"), wxDefaultPosition, wxSize(20, 20));
		Connect(m_btn_add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersMgrWidget::OnAddLayer));
		topSizer->Add(m_btn_add);
	}
	topSizer->AddSpacer(2);
	{
		m_btn_del = new wxButton(this, wxID_ANY, wxT("-"), wxDefaultPosition, wxSize(20, 20));
		Connect(m_btn_del->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersMgrWidget::OnDelLayer));
		topSizer->Add(m_btn_del);
	}
	topSizer->AddSpacer(5);
	{
		m_btn_up = new wxButton(this, wxID_ANY, wxT("up"), wxDefaultPosition, wxSize(20, 20));
		Connect(m_btn_up->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersMgrWidget::OnLayerUp));
		topSizer->Add(m_btn_up);
	}
	topSizer->AddSpacer(2);
	{
		m_btn_down = new wxButton(this, wxID_ANY, wxT("down"), wxDefaultPosition, wxSize(40, 20));
		Connect(m_btn_down->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersMgrWidget::OnLayerDown));
		topSizer->Add(m_btn_down);
	}
	return topSizer;
}

void LayersMgrWidget::OnAddLayer(wxCommandEvent& event)
{
	AddLayer(new Layer);
}

void LayersMgrWidget::OnDelLayer(wxCommandEvent& event)
{
	int index = GetEditLayerIndex();

	m_layers_sz->Detach(m_layers.size() - 1 - index);
	delete m_layers[index]->GetLayer();
	delete m_layers[index];
	m_layers.erase(m_layers.begin() + index);

	if (index < static_cast<int>(m_layers.size()))
		SetEditable(m_layers[index]);
	else
		SetEditable(m_layers.back());

	if (m_layers.empty())
		m_edited_layer = NULL;

	if (m_layers.size() == 1)
		m_btn_del->Enable(false);

	m_parent->Fit();

	wxWindow* root = m_parent;
	while (root->GetParent())
		root = root->GetParent();
	root->Refresh(true);
}

void LayersMgrWidget::OnLayerUp(wxCommandEvent& event)
{
	int index = GetEditLayerIndex();

	LayerWidget* layer = m_layers[index];
	m_layers[index] = m_layers[index + 1];
	m_layers[index + 1] = layer;

 	m_layers_sz->Remove(m_layers.size() - 1 - index);
 	m_layers_sz->Insert(m_layers.size() - 1 - (index + 1), layer);

	if (index + 1 == m_layers.size() - 1)
	{
		m_btn_down->Enable(true);
		m_btn_up->Enable(false);
	}
	else
	{
		m_btn_down->Enable(true);
		m_btn_up->Enable(true);
	}

	SetCanvasDirtySJ::Instance()->SetDirty();

	Layout();
	Refresh(true);
}

void LayersMgrWidget::OnLayerDown(wxCommandEvent& event)
{
	int index = GetEditLayerIndex();

	LayerWidget* layer = m_layers[index];
	m_layers[index] = m_layers[index - 1];
	m_layers[index - 1] = layer;

	m_layers_sz->Remove(m_layers.size() - 1 - index);
	m_layers_sz->Insert(m_layers.size() - 1 - (index - 1), layer);

	if (index - 1 == 0)
	{
		m_btn_down->Enable(false);
		m_btn_up->Enable(true);

		// debug fix
		SetEditable(layer);
	}
	else
	{
		m_btn_down->Enable(true);
		m_btn_up->Enable(true);
	}

	SetCanvasDirtySJ::Instance()->SetDirty();

	Layout();
	Refresh(true);
}

int LayersMgrWidget::GetEditLayerIndex() const
{
	int index = 0;
	for (int n = m_layers.size(); index < n; ++index) {
		if (m_layers[index]->GetLayer() == m_edited_layer)
			break;
	}
	assert(index != m_layers.size());

	return index;
}

//////////////////////////////////////////////////////////////////////////
// class LayersMgrWidget::LayerWidget
//////////////////////////////////////////////////////////////////////////

LayersMgrWidget::LayerWidget::
LayerWidget(wxWindow* parent, Layer* layer)
	: wxPanel(parent)
	, m_editable(NULL)
	, m_visible(NULL)
	, m_selectable(NULL)
	, m_name(NULL)
	, m_layer(layer)
{
	InitLayout();
}

bool LayersMgrWidget::LayerWidget::
IsVisible() const
{
	return m_visible->IsChecked();
}

bool LayersMgrWidget::LayerWidget::
IsSelectable() const
{
	return m_selectable->IsChecked();
}

void LayersMgrWidget::LayerWidget::
SetEditable(bool editable)
{
	m_editable->SetValue(editable);
}

void LayersMgrWidget::LayerWidget::
OnSetVisible(wxCommandEvent& event)
{
	wxWindow* root = m_parent;
	while (root->GetParent())
		root = root->GetParent();

	root->Refresh(true);
}

void LayersMgrWidget::LayerWidget::
OnSetEditable(wxCommandEvent& event)
{
	static_cast<LayersMgrWidget*>(m_parent)->SetEditable(this);
}

void LayersMgrWidget::LayerWidget::
OnSetSelectable(wxCommandEvent& event)
{
	
}

void LayersMgrWidget::LayerWidget::
OnSetName(wxCommandEvent& event)
{
	m_layer->name = event.GetString().ToStdString();
}

void LayersMgrWidget::LayerWidget::
InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_editable = new wxRadioButton(this, wxID_ANY, wxEmptyString);
	m_editable->SetValue(true);
	Connect(m_editable->GetId(), wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(LayerWidget::OnSetEditable));
	sizer->Add(m_editable);

	sizer->AddSpacer(5);

	m_visible = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_visible->SetValue(true);
	Connect(m_visible->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(LayerWidget::OnSetVisible));
	sizer->Add(m_visible);

	sizer->AddSpacer(5);

	m_selectable = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_selectable->SetValue(true);
	Connect(m_selectable->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(LayerWidget::OnSetSelectable));
	sizer->Add(m_selectable);

	sizer->AddSpacer(5);

	m_name = new wxTextCtrl(this, wxID_ANY, m_layer->name, wxDefaultPosition, wxSize(50, -1));
	Connect(m_name->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(LayerWidget::OnSetName));
	sizer->Add(m_name);

	SetSizer(sizer);
}

}
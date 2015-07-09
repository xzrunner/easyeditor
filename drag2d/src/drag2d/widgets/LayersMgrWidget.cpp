#include "LayersMgrWidget.h"

#include "common/tools.h"
#include "dataset/Layer.h"
#include "view/EditPanel.h"
#include "view/IStageCanvas.h"

namespace d2d
{

LayersMgrWidget::LayersMgrWidget(wxWindow* parent, d2d::EditPanel* stage)
	: wxPanel(parent)
	, m_stage(stage)
	, m_editedLayer(NULL)
	, m_layersSizer(NULL)
	, m_btnAdd(NULL), m_btnDel(NULL)
	, m_btnUp(NULL), m_btnDown(NULL)
{
	initLayout();
	onAddLayer(wxCommandEvent());

	m_btnDel->Enable(false);
	m_btnDown->Enable(false);
	m_btnUp->Enable(false);
}

LayersMgrWidget::~LayersMgrWidget()
{
	for_each(m_layers.begin(), m_layers.end(), DeletePointerFunctor<LayerWidget>());
	m_layers.clear();
	m_editedLayer = NULL;
}

void LayersMgrWidget::traverseEditableLayers(IVisitor& visitor)
{
	if (m_editedLayer)
	{
		bool hasNext;
		visitor.Visit(m_editedLayer, hasNext);
	}
}

void LayersMgrWidget::traverseVisibleLayers(IVisitor& visitor)
{
	std::vector<LayerWidget*>::const_iterator itr = m_layers.begin();
	for ( ; itr != m_layers.end(); ++itr)
	{
		if ((*itr)->isVisible())
		{
			bool hasNext;
			visitor.Visit((*itr)->getLayer(), hasNext);
			if (!hasNext) break;
		}
	}
}

void LayersMgrWidget::traverseSelectableLayers(IVisitor& visitor)
{
	std::vector<LayerWidget*>::const_iterator itr = m_layers.begin();
	for ( ; itr != m_layers.end(); ++itr)
	{
		if ((*itr)->isSelectable())
		{
			bool hasNext;
			visitor.Visit((*itr)->getLayer(), hasNext);
			if (!hasNext) break;
		}
	}
}

void LayersMgrWidget::traverseAllLayers(IVisitor& visitor)
{
	std::vector<LayerWidget*>::const_iterator itr = m_layers.begin();
	for ( ; itr != m_layers.end(); ++itr)
	{
		bool hasNext;
		visitor.Visit((*itr)->getLayer(), hasNext);
		if (!hasNext) break;
	}
}

void LayersMgrWidget::setEditable(LayerWidget* layer)
{
	m_editedLayer = layer->getLayer();
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
		m_layers[i]->setEditable(m_layers[i] == layer);

	if (layer == m_layers.front() && layer == m_layers.back())
	{
		m_btnDown->Enable(false);
		m_btnUp->Enable(false);
	}
	else if (layer == m_layers.front())
	{
		m_btnDown->Enable(false);
		m_btnUp->Enable(true);
	}
	else if (layer == m_layers.back())
	{
		m_btnDown->Enable(true);
		m_btnUp->Enable(false);
	}
	else
	{
		m_btnDown->Enable(true);
		m_btnUp->Enable(true);
	}
}

void LayersMgrWidget::getAllLayers(std::vector<Layer*>& layers) const
{
	layers.reserve(m_layers.size());
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
		layers.push_back(m_layers[i]->getLayer());
}

void LayersMgrWidget::addLayer(Layer* layer)
{
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
		m_layers[i]->setEditable(false);
	
	LayerWidget* wrapper = new LayerWidget(this, layer);
	m_editedLayer = wrapper->getLayer();
	m_layersSizer->Insert(0, wrapper);
	m_layers.push_back(wrapper);

	m_btnDel->Enable(true);
	m_btnDown->Enable(true);
	m_btnUp->Enable(false);

	m_parent->Fit();
}

void LayersMgrWidget::addLayerBottom(Layer* layer)
{
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
		m_layers[i]->setEditable(false);

	LayerWidget* wrapper = new LayerWidget(this, layer);
	m_editedLayer = wrapper->getLayer();
	m_layersSizer->Insert(m_layers.size(), wrapper);
	m_layers.insert(m_layers.begin(), wrapper);

	m_btnDel->Enable(true);
	m_btnDown->Enable(true);
	m_btnUp->Enable(false);

	m_parent->Fit();
}

void LayersMgrWidget::clear()
{
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
		m_layersSizer->Detach(0);

	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		delete m_layers[i]->getLayer();
		delete m_layers[i];
	}
	m_layers.clear();

	m_editedLayer = NULL;

	m_btnDel->Enable(false);
	m_btnUp->Enable(false);
	m_btnDown->Enable(false);

	m_parent->GetParent()->Layout();
}

void LayersMgrWidget::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Layers"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	topSizer->AddSpacer(5);
	topSizer->Add(initButtonsPanel());
	topSizer->AddSpacer(2);
	topSizer->Add(m_layersSizer = initLayersPanel());

	SetSizer(topSizer);
}

wxSizer* LayersMgrWidget::initLayersPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxEmptyString);
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	return topSizer;
}

wxSizer* LayersMgrWidget::initButtonsPanel()
{
	wxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	{
		m_btnAdd = new wxButton(this, wxID_ANY, wxT("+"), wxDefaultPosition, wxSize(20, 20));
		Connect(m_btnAdd->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersMgrWidget::onAddLayer));
		topSizer->Add(m_btnAdd);
	}
	topSizer->AddSpacer(2);
	{
		m_btnDel = new wxButton(this, wxID_ANY, wxT("-"), wxDefaultPosition, wxSize(20, 20));
		Connect(m_btnDel->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersMgrWidget::onDelLayer));
		topSizer->Add(m_btnDel);
	}
	topSizer->AddSpacer(5);
	{
		m_btnUp = new wxButton(this, wxID_ANY, wxT("up"), wxDefaultPosition, wxSize(20, 20));
		Connect(m_btnUp->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersMgrWidget::onLayerUp));
		topSizer->Add(m_btnUp);
	}
	topSizer->AddSpacer(2);
	{
		m_btnDown = new wxButton(this, wxID_ANY, wxT("down"), wxDefaultPosition, wxSize(40, 20));
		Connect(m_btnDown->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersMgrWidget::onLayerDown));
		topSizer->Add(m_btnDown);
	}
	return topSizer;
}

void LayersMgrWidget::onAddLayer(wxCommandEvent& event)
{
	addLayer(new Layer);
}

void LayersMgrWidget::onDelLayer(wxCommandEvent& event)
{
	int index = getEditLayerIndex();

	m_layersSizer->Detach(m_layers.size() - 1 - index);
	delete m_layers[index]->getLayer();
	delete m_layers[index];
	m_layers.erase(m_layers.begin() + index);

	if (index < m_layers.size())
		setEditable(m_layers[index]);
	else
		setEditable(m_layers.back());

	if (m_layers.empty())
		m_editedLayer = NULL;

	if (m_layers.size() == 1)
		m_btnDel->Enable(false);

	m_parent->Fit();

	wxWindow* root = m_parent;
	while (root->GetParent())
		root = root->GetParent();
	root->Refresh(true);
}

void LayersMgrWidget::onLayerUp(wxCommandEvent& event)
{
	int index = getEditLayerIndex();

	LayerWidget* layer = m_layers[index];
	m_layers[index] = m_layers[index + 1];
	m_layers[index + 1] = layer;

 	m_layersSizer->Remove(m_layers.size() - 1 - index);
 	m_layersSizer->Insert(m_layers.size() - 1 - (index + 1), layer);

	if (index + 1 == m_layers.size() - 1)
	{
		m_btnDown->Enable(true);
		m_btnUp->Enable(false);
	}
	else
	{
		m_btnDown->Enable(true);
		m_btnUp->Enable(true);
	}

	m_stage->GetCanvas()->SetDirty();

	Layout();
	Refresh(true);
}

void LayersMgrWidget::onLayerDown(wxCommandEvent& event)
{
	int index = getEditLayerIndex();

	LayerWidget* layer = m_layers[index];
	m_layers[index] = m_layers[index - 1];
	m_layers[index - 1] = layer;

	m_layersSizer->Remove(m_layers.size() - 1 - index);
	m_layersSizer->Insert(m_layers.size() - 1 - (index - 1), layer);

	if (index - 1 == 0)
	{
		m_btnDown->Enable(false);
		m_btnUp->Enable(true);

		// debug fix
		setEditable(layer);
	}
	else
	{
		m_btnDown->Enable(true);
		m_btnUp->Enable(true);
	}

	m_stage->GetCanvas()->SetDirty();

	Layout();
	Refresh(true);
}

int LayersMgrWidget::getEditLayerIndex() const
{
	int index = 0;
	for (size_t n = m_layers.size(); index < n; ++index)
	{
		if (m_layers[index]->getLayer() == m_editedLayer)
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
	initLayout();
}

bool LayersMgrWidget::LayerWidget::
isVisible() const
{
	return m_visible->IsChecked();
}

bool LayersMgrWidget::LayerWidget::
isSelectable() const
{
	return m_selectable->IsChecked();
}

void LayersMgrWidget::LayerWidget::
setEditable(bool editable)
{
	m_editable->SetValue(editable);
}

void LayersMgrWidget::LayerWidget::
onSetVisible(wxCommandEvent& event)
{
	wxWindow* root = m_parent;
	while (root->GetParent())
		root = root->GetParent();

	root->Refresh(true);
}

void LayersMgrWidget::LayerWidget::
onSetEditable(wxCommandEvent& event)
{
	static_cast<LayersMgrWidget*>(m_parent)->setEditable(this);
}

void LayersMgrWidget::LayerWidget::
onSetSelectable(wxCommandEvent& event)
{
	
}

void LayersMgrWidget::LayerWidget::
onSetName(wxCommandEvent& event)
{
	m_layer->setName(event.GetString());
}

void LayersMgrWidget::LayerWidget::
initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_editable = new wxRadioButton(this, wxID_ANY, wxEmptyString);
	m_editable->SetValue(true);
	Connect(m_editable->GetId(), wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(LayerWidget::onSetEditable));
	sizer->Add(m_editable);

	sizer->AddSpacer(5);

	m_visible = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_visible->SetValue(true);
	Connect(m_visible->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(LayerWidget::onSetVisible));
	sizer->Add(m_visible);

	sizer->AddSpacer(5);

	m_selectable = new wxCheckBox(this, wxID_ANY, wxEmptyString);
	m_selectable->SetValue(true);
	Connect(m_selectable->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(LayerWidget::onSetSelectable));
	sizer->Add(m_selectable);

	sizer->AddSpacer(5);

	m_name = new wxTextCtrl(this, wxID_ANY, m_layer->getName(), wxDefaultPosition, wxSize(50, -1));
	Connect(m_name->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(LayerWidget::onSetName));
	sizer->Add(m_name);

	SetSizer(sizer);
}

} // d2d
#include "TimeLinePanel.h"
#include "LayersPanel.h"
#include "KeysPanel.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include <wx/splitter.h>

namespace eanim
{

const float TimeLinePanel::SASH_GRAVITY_VERT = 0.1f;

TimeLinePanel::TimeLinePanel(wxWindow* parent)
	: wxPanel(parent)
{
	m_layersMgr = new LayersMgr;
	initLayout();
}

TimeLinePanel::~TimeLinePanel()
{
	delete m_layersMgr;
}

void TimeLinePanel::loadFromTextFile(std::ifstream& fin)
{
	m_layersMgr->loadFromTextFile(fin);
}

void TimeLinePanel::storeToTextFile(std::ofstream& fout) const
{
	m_layersMgr->storeToTextFile(fout);
}

LayersMgr* TimeLinePanel::getLayerMgr() const
{
	return m_layersMgr;
}

Layer* TimeLinePanel::getCurrLayer() const
{
	return m_layersMgr->getLayer(m_layersPanel->getSelectedLayer());
}

int TimeLinePanel::getCurrFrame() const
{
	return m_keysPanel->getCurrPos();
}

int TimeLinePanel::getFPSVal() const
{
	return m_layersPanel->getFPSVal();
}

LayersPanel* TimeLinePanel::getLayersPanel()
{
	return m_layersPanel;
}

KeysPanel* TimeLinePanel::getKeysPanel()
{
	return m_keysPanel;
}

void TimeLinePanel::clear()
{
	m_layersMgr->clear();
	m_keysPanel->setSelectPos(-1, -1);
}

void TimeLinePanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	initTitle(sizer);
	initSplitterWindow(sizer);
	SetSizer(sizer);
}

void TimeLinePanel::initTitle(wxSizer* sizer)
{
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Time Line"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);
}

void TimeLinePanel::initSplitterWindow(wxSizer* sizer)
{
	wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
	m_layersPanel = new LayersPanel(splitter, m_layersMgr);
	m_keysPanel = new KeysPanel(splitter, m_layersMgr);
	splitter->SetSashGravity(SASH_GRAVITY_VERT);
	splitter->SplitVertically(m_layersPanel, m_keysPanel);
	sizer->Add(splitter, 1, wxEXPAND);
}

} // eanim
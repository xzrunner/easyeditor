#include "LayersMgrCMPT.h"

#include "operator/SelectShapesOP.h"
#include "operator/SelectSpritesOP.h"
#include "widgets/LayersMgrWidget.h"
#include "view/EditPanel.h"

namespace d2d
{

LayersMgrCMPT::LayersMgrCMPT(wxWindow* parent, const wxString& name, 
							 EditPanel* editPanel, MultiShapesImpl* shapesImpl,
							 ViewPanelMgr* view_panel_mgr)
	: AbstractEditCMPT(parent, name, editPanel)
	, m_layersMgr(NULL)
{
	m_editOP = new SelectShapesOP(editPanel, editPanel->GetStageImpl(), shapesImpl, view_panel_mgr);

	m_layersMgr = new LayersMgrWidget(this, editPanel);
	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_layersMgr);
}

LayersMgrCMPT::LayersMgrCMPT(wxWindow* parent, const wxString& name, 
							 EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
							 ViewPanelMgr* view_panel_mgr)
	 : AbstractEditCMPT(parent, name, editPanel)
	 , m_layersMgr(NULL)
{
	m_editOP = new SelectSpritesOP(editPanel, editPanel->GetStageImpl(), spritesImpl, view_panel_mgr);

	m_layersMgr = new LayersMgrWidget(this, editPanel);
	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_layersMgr);
}

wxSizer* LayersMgrCMPT::initLayout()
{
	return m_sizer;
}

} // d2d
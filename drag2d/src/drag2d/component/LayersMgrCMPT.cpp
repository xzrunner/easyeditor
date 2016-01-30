#include "LayersMgrCMPT.h"

#include "operator/SelectShapesOP.h"
#include "operator/SelectSpritesOP.h"
#include "widgets/LayersMgrWidget.h"

namespace d2d
{

LayersMgrCMPT::LayersMgrCMPT(wxWindow* parent, const wxString& name, 
							 wxWindow* stage_wnd, EditPanelImpl* stage, 
							 MultiShapesImpl* shapesImpl)
	: AbstractEditCMPT(parent, name, stage)
	, m_layersMgr(NULL)
{
	m_editOP = new SelectShapesOP(stage_wnd, stage, shapesImpl);

	m_layersMgr = new LayersMgrWidget(this);
	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_layersMgr);
}

LayersMgrCMPT::LayersMgrCMPT(wxWindow* parent, const wxString& name, 
							 wxWindow* stage_wnd, EditPanelImpl* stage, 
							 MultiSpritesImpl* spritesImpl)
	 : AbstractEditCMPT(parent, name, stage)
	 , m_layersMgr(NULL)
{
	m_editOP = new SelectSpritesOP(stage_wnd, stage, spritesImpl);

	m_layersMgr = new LayersMgrWidget(this);
	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_layersMgr);
}

wxSizer* LayersMgrCMPT::initLayout()
{
	return m_sizer;
}

}
#include "LayersMgrCMPT.h"
#include "SelectShapesOP.h"
#include "SelectSpritesOP.h"
#include "LayersMgrWidget.h"

namespace ee
{

LayersMgrCMPT::LayersMgrCMPT(wxWindow* parent, const std::string& name, 
							 wxWindow* stage_wnd, EditPanelImpl* stage, 
							 MultiShapesImpl* shapes_impl)
	: EditCMPT(parent, name, stage)
	, m_layersMgr(NULL)
{
	m_editop = new SelectShapesOP(stage_wnd, stage, shapes_impl);

	m_layersMgr = new LayersMgrWidget(this);
	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_layersMgr);
}

LayersMgrCMPT::LayersMgrCMPT(wxWindow* parent, const std::string& name, 
							 wxWindow* stage_wnd, EditPanelImpl* stage, 
							 MultiSpritesImpl* spritesImpl)
	 : EditCMPT(parent, name, stage)
	 , m_layersMgr(NULL)
{
	m_editop = new SelectSpritesOP(stage_wnd, stage, spritesImpl);

	m_layersMgr = new LayersMgrWidget(this);
	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_layersMgr);
}

wxSizer* LayersMgrCMPT::InitLayout()
{
	return m_sizer;
}

}
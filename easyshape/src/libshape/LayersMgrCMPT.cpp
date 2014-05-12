#include "LayersMgrCMPT.h"

namespace libshape
{

LayersMgrCMPT::LayersMgrCMPT(wxWindow* parent, const wxString& name, 
							 d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
							 d2d::PropertySettingPanel* propertyPanel)
	: d2d::AbstractEditCMPT(parent, name, editPanel)
	, m_layersMgr(NULL)
{
//	m_editOP = new SelectShapesOP(editPanel, shapesImpl, propertyPanel);
	m_editOP = new ZoomViewOP(editPanel, true);

	m_layersMgr = new LayersMgrWidget(this, editPanel);
	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_layersMgr);
}

LayersMgrCMPT::LayersMgrCMPT(wxWindow* parent, const wxString& name, 
							 EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
							 PropertySettingPanel* propertyPanel)
	 : AbstractEditCMPT(parent, name, editPanel)
	 , m_layersMgr(NULL)
{
	m_editOP = new SelectSpritesOP(editPanel, spritesImpl, propertyPanel);

	m_layersMgr = new LayersMgrWidget(this, editPanel);
	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_layersMgr);
}

wxSizer* LayersMgrCMPT::initLayout()
{
	return m_sizer;
}

} // d2d
#include "LayersMgrCMPT.h"

#include "operator/SelectShapesOP.h"
#include "operator/SelectSpritesOP.h"
#include "widgets/LayersMgrWidget.h"

namespace d2d
{

LayersMgrCMPT::LayersMgrCMPT(wxWindow* parent, const wxString& name, 
							 EditPanel* editPanel, MultiShapesImpl* shapesImpl,
							 PropertySettingPanel* propertyPanel)
	: AbstractEditCMPT(parent, name, editPanel)
	, m_layersMgr(NULL)
{
	m_editOP = new SelectShapesOP(editPanel, shapesImpl, propertyPanel);

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
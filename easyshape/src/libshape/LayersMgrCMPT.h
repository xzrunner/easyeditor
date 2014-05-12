#ifndef _LIBSHAPE_LAYERS_MANAGER_CMPT_H_
#define _LIBSHAPE_LAYERS_MANAGER_CMPT_H_

#include <drag2d.h>

namespace libshape
{

class LayersMgrCMPT : public d2d::AbstractEditCMPT
{
public:
	LayersMgrCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* propertyPanel);
	LayersMgrCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl,
		d2d::PropertySettingPanel* propertyPanel);

	LayersMgrWidget* getLayersMgr() { return m_layersMgr; }

protected:
	virtual wxSizer* initLayout();

private:
	LayersMgrWidget* m_layersMgr;

	wxSizer* m_sizer;

}; // LayersMgrCMPT

}

#endif // _LIBSHAPE_LAYERS_MANAGER_CMPT_H_
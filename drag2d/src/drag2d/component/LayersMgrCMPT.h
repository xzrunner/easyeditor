#ifndef _DRAG2D_LAYERS_MGR_CMPT_H_
#define _DRAG2D_LAYERS_MGR_CMPT_H_

#include "AbstractEditCMPT.h"

namespace d2d
{

class MultiShapesImpl;
class MultiSpritesImpl;
class LayersMgrWidget;
class ViewPanelMgr;

class LayersMgrCMPT : public AbstractEditCMPT
{
public:
	LayersMgrCMPT(wxWindow* parent, const wxString& name,
		wxWindow* stage_wnd, EditPanelImpl* stage, 
		MultiShapesImpl* shapesImpl, ViewPanelMgr* view_panel_mgr);
	LayersMgrCMPT(wxWindow* parent, const wxString& name,
		wxWindow* stage_wnd, EditPanelImpl* stage, 
		MultiSpritesImpl* spritesImpl, ViewPanelMgr* view_panel_mgr);

	LayersMgrWidget* getLayersMgr() { return m_layersMgr; }

protected:
	virtual wxSizer* initLayout();

private:
	LayersMgrWidget* m_layersMgr;

	wxSizer* m_sizer;

}; // LayersMgrCMPT

}

#endif // _DRAG2D_LAYERS_MGR_CMPT_H_
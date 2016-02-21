#ifndef _EASYEDITOR_LAYERS_MGR_CMPT_H_
#define _EASYEDITOR_LAYERS_MGR_CMPT_H_

#include "EditCMPT.h"

namespace ee
{

class MultiShapesImpl;
class MultiSpritesImpl;
class LayersMgrWidget;

class LayersMgrCMPT : public EditCMPT
{
public:
	LayersMgrCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd, 
		EditPanelImpl* stage, MultiShapesImpl* shapes_impl);
	LayersMgrCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd, 
		EditPanelImpl* stage, MultiSpritesImpl* sprites_impl);

	LayersMgrWidget* getLayersMgr() { return m_layersMgr; }

protected:
	virtual wxSizer* InitLayout();

private:
	LayersMgrWidget* m_layersMgr;

	wxSizer* m_sizer;

}; // LayersMgrCMPT

}

#endif // _EASYEDITOR_LAYERS_MGR_CMPT_H_
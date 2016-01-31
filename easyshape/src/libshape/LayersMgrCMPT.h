#ifndef _LIBSHAPE_LAYERS_MANAGER_CMPT_H_
#define _LIBSHAPE_LAYERS_MANAGER_CMPT_H_



namespace libshape
{

class LayersMgrCMPT : public ee::AbstractEditCMPT
{
public:
	LayersMgrCMPT(wxWindow* parent, const wxString& name,
		ee::EditPanel* editPanel, ee::MultiShapesImpl* shapesImpl,
		ee::PropertySettingPanel* propertyPanel);
	LayersMgrCMPT(wxWindow* parent, const wxString& name,
		ee::EditPanel* editPanel, ee::MultiSpritesImpl* spritesImpl,
		ee::PropertySettingPanel* propertyPanel);

	LayersMgrWidget* getLayersMgr() { return m_layersMgr; }

protected:
	virtual wxSizer* initLayout();

private:
	LayersMgrWidget* m_layersMgr;

	wxSizer* m_sizer;

}; // LayersMgrCMPT

}

#endif // _LIBSHAPE_LAYERS_MANAGER_CMPT_H_
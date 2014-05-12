#pragma once

#include "AbstractEditCMPT.h"

namespace d2d
{
	class MultiShapesImpl;
	class MultiSpritesImpl;
	class LayersMgrWidget;
	class PropertySettingPanel;

	class LayersMgrCMPT : public AbstractEditCMPT
	{
	public:
		LayersMgrCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			PropertySettingPanel* propertyPanel);
		LayersMgrCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
			PropertySettingPanel* propertyPanel);

		LayersMgrWidget* getLayersMgr() { return m_layersMgr; }

	protected:
		virtual wxSizer* initLayout();

	private:
		LayersMgrWidget* m_layersMgr;

		wxSizer* m_sizer;

	}; // LayersMgrCMPT
}


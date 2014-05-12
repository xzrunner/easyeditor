#pragma once
#include <wx/wx.h>

namespace eanim
{
	class TimeLinePanel;
	class LayersMgr;
	class LayersPropertyWidget;
	class LayersContentWidget;

	class LayersPanel : public wxPanel
	{
	public:
		LayersPanel(wxWindow* parent, LayersMgr* layersMgr);

		void onAddLayer();
		void onDelLayer();
		void onPlay();

		void setSelectedLayer(int index);
		int getSelectedLayer() const;

		void refreshKeysPanel() const;

		int getFPSVal() const; 

	private:
		void initLayout();

	private:
		LayersMgr* m_layersMgr;

		LayersPropertyWidget* m_propertyWidget;
		LayersContentWidget* m_contentWidget;

		int m_selected;

	}; // LayersPanel
}


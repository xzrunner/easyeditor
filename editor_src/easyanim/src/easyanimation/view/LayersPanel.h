#pragma once

#include <wx/wx.h>

namespace eanim
{
	class LayersPropertyWidget;

	class LayersPanel : public wxPanel
	{
	public:
		LayersPanel(wxWindow* parent);

		void insertLayer();
		void removeLayer();
//		void onPlay();

		void setFPS(int fps);

	private:
		void initLayout();

	private:
		LayersPropertyWidget* m_layersProperty;

	}; // LayersPanel
}


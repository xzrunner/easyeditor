#pragma once

#include <wx/wx.h>

namespace eanim
{
	class Controller;
	class LayersPropertyWidget;

	class LayersPanel : public wxPanel
	{
	public:
		LayersPanel(wxWindow* parent, Controller* ctrl);

		void insertLayer();
		void removeLayer();
//		void onPlay();

		void setFPS(int fps);

	private:
		void initLayout();

	private:
		Controller* m_ctrl;

		LayersPropertyWidget* m_layersProperty;

		wxWindow* m_contentPanel;

	}; // LayersPanel
}


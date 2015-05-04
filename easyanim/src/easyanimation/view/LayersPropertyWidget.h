#pragma once

#include <wx/wx.h>

namespace eanim
{

	class Controller;

	class LayersPropertyWidget : public wxPanel
	{
	public:
		LayersPropertyWidget(wxWindow* parent, Controller* ctrl);

		void setFPS(int fps);

	private:
		void initLayout();

		void onAddLayer(wxCommandEvent& event);
		void onDelLayer(wxCommandEvent& event);
//		void onPlay(wxCommandEvent& event);
		void onChangeFps(wxSpinEvent& event);

	private:
		Controller* m_ctrl;

		wxSpinCtrl* m_fps;

	}; // LayersPropertyWidget
}


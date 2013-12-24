#pragma once

#include <wx/wx.h>

namespace eanim
{
	class LayersPropertyWidget : public wxPanel
	{
	public:
		LayersPropertyWidget(wxWindow* parent);

		void setFPS(int fps);

	private:
		void initLayout();

		void onAddLayer(wxCommandEvent& event);
		void onDelLayer(wxCommandEvent& event);
//		void onPlay(wxCommandEvent& event);
		void onChangeFps(wxSpinEvent& event);

	private:
		wxSpinCtrl* m_fps;

	}; // LayersPropertyWidget
}


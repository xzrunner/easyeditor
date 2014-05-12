#pragma once
#include <wx/wx.h>

namespace eanim
{
	class LayersPanel;

	class LayersPropertyWidget : public wxPanel
	{
	public:
		LayersPropertyWidget(LayersPanel* parent);

		int getFPSVal() const;

	private:
		void initLayout();

		void onAddLayer(wxCommandEvent& event);
		void onDelLayer(wxCommandEvent& event);
//		void onPlay(wxCommandEvent& event);

	private:
		LayersPanel* m_parent;
		wxButton *m_btnAdd, *m_btnDel;
		wxButton* m_btnPlay;
		wxSpinCtrl* m_fpsSpin;

	}; // LayersPropertyWidget
}


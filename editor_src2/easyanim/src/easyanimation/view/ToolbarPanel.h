#pragma once

#include <drag2d.h>

namespace eanim
{
	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent);

		void addAnimChoice(const std::vector<std::string>& choices);

	protected:
		virtual wxSizer* initLayout();

	private:
		void onLoadAllFrameImages(wxCommandEvent& event);
		void onFillingFrames(wxCommandEvent& event);

		void onChangeAnim(wxCommandEvent& event);

	private:
		wxSpinCtrl* m_filling;

		wxRadioBox* m_animChoice;

	}; // ToolbarPanel
}


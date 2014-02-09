#pragma once

#include <drag2d.h>

namespace eanim
{
	class StagePanel;

	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent, StagePanel* stage);

		void addAnimChoice(const std::vector<std::string>& choices);

	protected:
		virtual wxSizer* initLayout();

	private:
		void onChangeAnim(wxCommandEvent& event);

	private:
		wxRadioBox* m_animChoice;

	}; // ToolbarPanel
}


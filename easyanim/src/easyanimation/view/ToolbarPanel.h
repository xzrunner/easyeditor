#pragma once

#include <drag2d.h>

namespace eanim
{
	class Controller;
	class StagePanel;

	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent, StagePanel* stage, 
			d2d::PropertySettingPanel* property, 
			d2d::ViewPanelMgr* view_panel_mgr,
			bool vertical, Controller* ctrl);

		void addAnimChoice(const std::vector<std::string>& choices);

	protected:
		virtual wxSizer* initLayout();

	private:
		void onChangeAnim(wxCommandEvent& event);

	private:
		Controller* m_ctrl;

		wxRadioBox* m_animChoice;

	}; // ToolbarPanel
}


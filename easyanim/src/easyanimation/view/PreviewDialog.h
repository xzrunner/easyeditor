#pragma once

#include <wx/wx.h>
#include <drag2d.h>
#include <easyanim.h>

#include "PreviewSettings.h"

namespace eanim
{
	class PreviewDialog : public wxDialog
	{
	public:
		PreviewDialog(wxWindow* parent);
		~PreviewDialog();

	private:
		void initLayout();

		void buildToolBar(wxSizer* topSizer);
		void buildEditPanel(wxSizer* topSizer);

		void onSetCirculate(wxCommandEvent& event);
		void onSetStop(wxCommandEvent& event);

	private:
		d2d::EditPanel* m_stage;

		PlaySettings m_settings;

		d2d::PlayControl m_control;

	}; // PreviewDialog
}


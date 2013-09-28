#pragma once

#include <drag2d.h>

namespace coceditor
{
namespace complex
{
	class StagePanel;

	class Dialog : public wxDialog
	{
	public:
		Dialog(wxWindow* parent, d2d::ComplexSymbol* complexSymbol,
			d2d::PropertySettingPanel* propertyPanel);
		virtual ~Dialog();

	private:
		void onSize(wxSizeEvent& event);

		void initLayout(d2d::ComplexSymbol* complexSymbol);

		void refreshSpritesBounding();

	private:
		StagePanel* m_editPanel;

		d2d::ComplexSymbol* m_complexSymbol;

		d2d::PropertySettingPanel* m_propertyPanel;

		DECLARE_EVENT_TABLE()

	}; // Dialog
}
}


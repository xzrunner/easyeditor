#pragma once



namespace coceditor
{
namespace complex
{
	class StagePanel;

	class Dialog : public wxDialog
	{
	public:
		Dialog(wxWindow* parent, complex::Symbol* complexSymbol,
			ee::PropertySettingPanel* propertyPanel);
		virtual ~Dialog();

	private:
		void onSize(wxSizeEvent& event);

		void initLayout(complex::Symbol* complexSymbol);

		void refreshSpritesBounding();

	private:
		StagePanel* m_editPanel;

		complex::Symbol* m_complexSymbol;

		ee::PropertySettingPanel* m_propertyPanel;

		DECLARE_EVENT_TABLE()

	}; // Dialog
}
}


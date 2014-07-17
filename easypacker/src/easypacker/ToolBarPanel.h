#ifndef EPACKER_TOOLBAR_PANEL_H
#define EPACKER_TOOLBAR_PANEL_H

#include <drag2d.h>

namespace epacker
{
	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		enum IMG_TYPE
		{
			e_unknow = 0,
			e_bmp,
			e_jpg,
			e_png
		};

	public:
		ToolbarPanel(wxWindow* parent);

		IMG_TYPE getImgType() const;

		void setSize(int width, int height);

	protected:
		virtual wxSizer* initLayout();

	private:
		void initSizeSettingPanel(wxSizer* topSizer);
		void initSettingsPanel(wxSizer* topSizer);
		void initFormatChoicePanel(wxSizer* topSizer);

		void onChangeOutputImageSize(wxCommandEvent& event);
		void onChangePadding(wxSpinEvent& event);
		void onChangeScale(wxSpinEvent& event);
		void onRearrange(wxCommandEvent& event);
		void onLoadLibraryList(wxCommandEvent& event);
		void onChangeAutoArrange(wxCommandEvent& event);
		void onChangePremultipliedAlpha(wxCommandEvent& event);

	private:
		static const int BASE_EDGE = 32;
		static const int TOTLE_EDGE_TYPES = 7;

	private:
		wxChoice *m_widthChoice, *m_heightChoice;
		wxRadioBox* m_formatChoice;

		wxCheckBox* m_auto_arrage;

	}; // ToolbarPanel
}

#endif // EPACKER_TOOLBAR_PANEL_H
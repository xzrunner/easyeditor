#ifndef _EASYTEXPACKER_IMAGE_TOOLBAR_PAGE_H_
#define _EASYTEXPACKER_IMAGE_TOOLBAR_PAGE_H_

#include "const.h"

#include <wx/wx.h>

namespace etexpacker
{

class StagePanel;

class ImageToolbarPage : public wxPanel
{
public:
	ImageToolbarPage(wxWindow* parent, StagePanel* stage);

	IMG_TYPE GetImgType() const;

	void SetSize(int width, int height);

private:
	void InitLayout();

	void InitSizeSettingPanel(wxSizer* topSizer);
	void InitSettingsPanel(wxSizer* topSizer);
	void InitFormatChoicePanel(wxSizer* topSizer);

	void OnChangeOutputImageSize(wxCommandEvent& event);
	void OnChangePadding(wxSpinEvent& event);
	void OnChangeScale(wxSpinEvent& event);
	void OnChangeExtrude(wxSpinEvent& event);
	void OnRearrange(wxCommandEvent& event);
	void OnLoadLibraryList(wxCommandEvent& event);
	void OnChangeAutoArrange(wxCommandEvent& event);
	void OnChangePremultipliedAlpha(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	wxChoice *m_width_choice, *m_height_choice;
	wxRadioBox* m_format_choice;

	wxCheckBox* m_auto_arrage;

}; // ImageToolbarPage

}

#endif // _EASYTEXPACKER_IMAGE_TOOLBAR_PAGE_H_
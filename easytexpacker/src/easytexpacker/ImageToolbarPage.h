#ifndef _EASYTEXPACKER_IMAGE_TOOLBAR_PAGE_H_
#define _EASYTEXPACKER_IMAGE_TOOLBAR_PAGE_H_

#include <drag2d.h>

#include "const.h"

namespace etexpacker
{

class StagePanel;

class ImageToolbarPage : public wxPanel
{
public:
	ImageToolbarPage(wxWindow* parent, StagePanel* stage);

	IMG_TYPE getImgType() const;

	void setSize(int width, int height);

private:
	void InitLayout();

	void initSizeSettingPanel(wxSizer* topSizer);
	void initSettingsPanel(wxSizer* topSizer);
	void initFormatChoicePanel(wxSizer* topSizer);

	void onChangeOutputImageSize(wxCommandEvent& event);
	void onChangePadding(wxSpinEvent& event);
	void onChangeScale(wxSpinEvent& event);
	void onChangeExtrude(wxSpinEvent& event);
	void onRearrange(wxCommandEvent& event);
	void onLoadLibraryList(wxCommandEvent& event);
	void onChangeAutoArrange(wxCommandEvent& event);
	void onChangePremultipliedAlpha(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	wxChoice *m_widthChoice, *m_heightChoice;
	wxRadioBox* m_formatChoice;

	wxCheckBox* m_auto_arrage;

}; // ImageToolbarPage

}

#endif // _EASYTEXPACKER_IMAGE_TOOLBAR_PAGE_H_
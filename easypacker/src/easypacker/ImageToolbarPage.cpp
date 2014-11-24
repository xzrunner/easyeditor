#include "ImageToolbarPage.h"
#include "Context.h"
#include "StagePanel.h"

namespace epacker
{

static const int BASE_EDGE = 32;
static const int TOTLE_EDGE_TYPES = 7;

ImageToolbarPage::ImageToolbarPage(wxWindow* parent, StagePanel* stage)
	: wxPanel(parent)
	, m_stage(stage)
{
	InitLayout();

	onChangeOutputImageSize(wxCommandEvent());
}

void ImageToolbarPage::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->AddSpacer(10);
	initSizeSettingPanel(sizer);
	sizer->AddSpacer(20);
	initSettingsPanel(sizer);
	sizer->AddSpacer(20);
	initFormatChoicePanel(sizer);
	sizer->AddSpacer(20);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Reset"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImageToolbarPage::onRearrange));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Load All"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImageToolbarPage::onLoadLibraryList));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);

	SetSizer(sizer);
}

IMG_TYPE ImageToolbarPage::getImgType() const
{
	switch (m_formatChoice->GetSelection())
	{
	case 0:
		return e_png;
	case 1:
		return e_jpg;
	case 2:
		return e_bmp;
	default:
		return e_unknow;
	}
}

inline int selection(int a)
{
	int rval = 1;
	while (!(a & 0x01)) 
	{
		a >>= 1;
		++rval;
	}
	rval -= 6;
	return rval;
}

void ImageToolbarPage::setSize(int width, int height)
{
	Context::Instance()->width = width;
	Context::Instance()->height = height;

	m_widthChoice->SetSelection(selection(width));
	m_heightChoice->SetSelection(selection(height));
	m_stage->arrangeAllSprites(true);
	m_stage->Refresh();
}

void ImageToolbarPage::initSizeSettingPanel(wxSizer* topSizer)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Size"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	wxString choices[TOTLE_EDGE_TYPES];
	for (size_t i = 0; i < TOTLE_EDGE_TYPES; ++i)
		choices[i] = wxString::FromDouble(BASE_EDGE * pow(2.0f, float(i)));

	wxSizer* widthSizer = new wxBoxSizer(wxHORIZONTAL);
	widthSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Width: ")));
	m_widthChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, TOTLE_EDGE_TYPES, choices);
	m_widthChoice->SetSelection(3);
	Connect(m_widthChoice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
		wxCommandEventHandler(ImageToolbarPage::onChangeOutputImageSize));
	widthSizer->Add(m_widthChoice);
	sizer->Add(widthSizer);

	wxSizer* heightSizer = new wxBoxSizer(wxHORIZONTAL);
	heightSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Height: ")));
	m_heightChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, TOTLE_EDGE_TYPES, choices);
	m_heightChoice->SetSelection(3);
	Connect(m_heightChoice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
		wxCommandEventHandler(ImageToolbarPage::onChangeOutputImageSize));
	heightSizer->Add(m_heightChoice);
	sizer->Add(heightSizer);

	topSizer->Add(sizer);
}

void ImageToolbarPage::initSettingsPanel(wxSizer* topSizer)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Settings"));
	wxBoxSizer* settingsSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Padding: ")));

		wxSpinCtrl* padding = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 0, 100, 0);
		Connect(padding->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ImageToolbarPage::onChangePadding));
		sizer->Add(padding);

		settingsSizer->Add(sizer);
	}
	settingsSizer->AddSpacer(10);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Scale: ")));

		wxSpinCtrl* scale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 10, 200, 100);
		Connect(scale->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ImageToolbarPage::onChangeScale));
		sizer->Add(scale);

		settingsSizer->Add(sizer);
	}
	settingsSizer->AddSpacer(10);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Extrude: ")));

		wxSpinCtrl* extrude = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 0, 10, 0);
		Connect(extrude->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ImageToolbarPage::onChangeScale));
		sizer->Add(extrude);

		settingsSizer->Add(sizer);
	}
	settingsSizer->AddSpacer(10);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("Auto Arrange"));
		check->SetValue(Context::Instance()->auto_arrange);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(ImageToolbarPage::onChangeAutoArrange));
		settingsSizer->Add(check);
	}
	settingsSizer->AddSpacer(10);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("Premultiplied Alpha"));
		check->SetValue(Context::Instance()->premultiplied_alpha);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(ImageToolbarPage::onChangePremultipliedAlpha));
		settingsSizer->Add(check);
	}
	topSizer->Add(settingsSizer);
}

void ImageToolbarPage::initFormatChoicePanel(wxSizer* topSizer)
{
	wxArrayString choices;
	choices.Add(wxT("png"));
	choices.Add(wxT("jpg"));
//	choices.Add(wxT("bmp"));	
	m_formatChoice = new wxRadioBox(this, wxID_ANY, wxT("Format"), 
		wxDefaultPosition, wxDefaultSize, choices, 2, wxRA_SPECIFY_COLS);
	topSizer->Add(m_formatChoice);
}

void ImageToolbarPage::onChangeOutputImageSize(wxCommandEvent& event)
{
	Context::Instance()->width = wxVariant(m_widthChoice->GetString(m_widthChoice->GetSelection())).GetInteger();
	Context::Instance()->height = wxVariant(m_heightChoice->GetString(m_heightChoice->GetSelection())).GetInteger();
	m_stage->arrangeAllSprites(true);
	m_stage->Refresh();
}

void ImageToolbarPage::onChangePadding(wxSpinEvent& event)
{
	Context::Instance()->padding = event.GetValue();
	m_stage->arrangeAllSprites(true);
	m_stage->Refresh();
}

void ImageToolbarPage::onChangeScale(wxSpinEvent& event)
{
	float scale = Context::Instance()->scale = event.GetValue() * 0.01f;

	std::vector<d2d::ISprite*> sprites;
	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->setScale(scale, scale);

	m_stage->arrangeAllSprites(true);
	m_stage->Refresh();
}

void ImageToolbarPage::onChangeExtrude(wxSpinEvent& event)
{
	Context::Instance()->extrude = event.GetValue();
}

void ImageToolbarPage::onRearrange(wxCommandEvent& event)
{
	m_stage->arrangeAllSprites(true);
	m_stage->Refresh();
}

void ImageToolbarPage::onLoadLibraryList(wxCommandEvent& event)
{
	m_stage->loadFromLibrary();
	m_stage->Refresh();
}

void ImageToolbarPage::onChangeAutoArrange(wxCommandEvent& event)
{
	Context::Instance()->auto_arrange = event.IsChecked();
}

void ImageToolbarPage::onChangePremultipliedAlpha(wxCommandEvent& event)
{
	Context::Instance()->premultiplied_alpha = event.IsChecked();
}

}
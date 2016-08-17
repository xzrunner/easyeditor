#include "ImageToolbarPage.h"
#include "Context.h"
#include "StagePanel.h"

#include <ee/panel_msg.h>
#include <ee/StringHelper.h>
#include <ee/Sprite.h>
#include <ee/FetchAllVisitor.h>

#include <wx/spinctrl.h>

namespace etexpacker
{

static const int BASE_EDGE = 32;
static const int TOTLE_EDGE_TYPES = 7;

ImageToolbarPage::ImageToolbarPage(wxWindow* parent, StagePanel* stage)
	: wxPanel(parent)
	, m_stage(stage)
{
	InitLayout();

	OnChangeOutputImageSize(wxCommandEvent());
}

void ImageToolbarPage::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->AddSpacer(10);
	InitSizeSettingPanel(sizer);
	sizer->AddSpacer(20);
	InitSettingsPanel(sizer);
	sizer->AddSpacer(20);
	InitFormatChoicePanel(sizer);
	sizer->AddSpacer(20);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Reset"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImageToolbarPage::OnRearrange));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Load All"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImageToolbarPage::OnLoadLibraryList));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);

	SetSizer(sizer);
}

IMG_TYPE ImageToolbarPage::GetImgType() const
{
	switch (m_format_choice->GetSelection())
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

void ImageToolbarPage::SetSize(int width, int height)
{
	Context::Instance()->width = width;
	Context::Instance()->height = height;

	m_width_choice->SetSelection(selection(width));
	m_height_choice->SetSelection(selection(height));
	m_stage->ArrangeAllSprites(true);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ImageToolbarPage::InitSizeSettingPanel(wxSizer* topSizer)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Size"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	wxString choices[TOTLE_EDGE_TYPES];
	for (size_t i = 0; i < TOTLE_EDGE_TYPES; ++i)
		choices[i] = ee::StringHelper::ToString(BASE_EDGE * pow(2.0f, float(i)));

	wxSizer* widthSizer = new wxBoxSizer(wxHORIZONTAL);
	widthSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Width: ")));
	m_width_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, TOTLE_EDGE_TYPES, choices);
	m_width_choice->SetSelection(3);
	Connect(m_width_choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
		wxCommandEventHandler(ImageToolbarPage::OnChangeOutputImageSize));
	widthSizer->Add(m_width_choice);
	sizer->Add(widthSizer);

	wxSizer* heightSizer = new wxBoxSizer(wxHORIZONTAL);
	heightSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Height: ")));
	m_height_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, TOTLE_EDGE_TYPES, choices);
	m_height_choice->SetSelection(3);
	Connect(m_height_choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
		wxCommandEventHandler(ImageToolbarPage::OnChangeOutputImageSize));
	heightSizer->Add(m_height_choice);
	sizer->Add(heightSizer);

	topSizer->Add(sizer);
}

void ImageToolbarPage::InitSettingsPanel(wxSizer* topSizer)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Settings"));
	wxBoxSizer* settingsSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Padding: ")));

		wxSpinCtrl* padding = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 0, 100, 0);
		Connect(padding->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ImageToolbarPage::OnChangePadding));
		sizer->Add(padding);

		settingsSizer->Add(sizer);
	}
	settingsSizer->AddSpacer(10);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Scale: ")));

		wxSpinCtrl* scale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 10, 200, 100);
		Connect(scale->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ImageToolbarPage::OnChangeScale));
		sizer->Add(scale);

		settingsSizer->Add(sizer);
	}
	settingsSizer->AddSpacer(10);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Extrude: ")));

		wxSpinCtrl* extrude = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 0, 10, 0);
		Connect(extrude->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ImageToolbarPage::OnChangeScale));
		sizer->Add(extrude);

		settingsSizer->Add(sizer);
	}
	settingsSizer->AddSpacer(10);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("Auto Arrange"));
		check->SetValue(Context::Instance()->auto_arrange);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(ImageToolbarPage::OnChangeAutoArrange));
		settingsSizer->Add(check);
	}
	settingsSizer->AddSpacer(10);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("Premultiplied Alpha"));
		check->SetValue(Context::Instance()->premultiplied_alpha);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(ImageToolbarPage::OnChangePremultipliedAlpha));
		settingsSizer->Add(check);
	}
	topSizer->Add(settingsSizer);
}

void ImageToolbarPage::InitFormatChoicePanel(wxSizer* topSizer)
{
	wxArrayString choices;
	choices.Add(wxT("png"));
	choices.Add(wxT("jpg"));
//	choices.Add(wxT("bmp"));	
	m_format_choice = new wxRadioBox(this, wxID_ANY, wxT("Format"), 
		wxDefaultPosition, wxDefaultSize, choices, 2, wxRA_SPECIFY_COLS);
	topSizer->Add(m_format_choice);
}

void ImageToolbarPage::OnChangeOutputImageSize(wxCommandEvent& event)
{
	Context::Instance()->width = wxVariant(m_width_choice->GetString(m_width_choice->GetSelection())).GetInteger();
	Context::Instance()->height = wxVariant(m_height_choice->GetString(m_height_choice->GetSelection())).GetInteger();
	m_stage->ArrangeAllSprites(true);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ImageToolbarPage::OnChangePadding(wxSpinEvent& event)
{
	Context::Instance()->padding = event.GetValue();
	m_stage->ArrangeAllSprites(true);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ImageToolbarPage::OnChangeScale(wxSpinEvent& event)
{
	float scale = Context::Instance()->scale = event.GetValue() * 0.01f;

	std::vector<ee::Sprite*> sprs;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->SetScale(sm::vec2(scale, scale));
	}

	m_stage->ArrangeAllSprites(true);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ImageToolbarPage::OnChangeExtrude(wxSpinEvent& event)
{
	Context::Instance()->extrude = event.GetValue();
}

void ImageToolbarPage::OnRearrange(wxCommandEvent& event)
{
	m_stage->ArrangeAllSprites(true);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ImageToolbarPage::OnLoadLibraryList(wxCommandEvent& event)
{
	m_stage->LoadFromLibrary();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ImageToolbarPage::OnChangeAutoArrange(wxCommandEvent& event)
{
	Context::Instance()->auto_arrange = event.IsChecked();
}

void ImageToolbarPage::OnChangePremultipliedAlpha(wxCommandEvent& event)
{
	Context::Instance()->premultiplied_alpha = event.IsChecked();
}

}
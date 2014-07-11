#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "Context.h"

using namespace epacker;

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: d2d::ToolbarPanel(parent, Context::Instance()->stage)
{
	SetSizer(initLayout());
	onChangeOutputImageSize(wxCommandEvent());
}

ToolbarPanel::IMG_TYPE ToolbarPanel::getImgType() const
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

void ToolbarPanel::setSize(int width, int height)
{
	Context::Instance()->width = width;
	Context::Instance()->height = height;

	m_widthChoice->SetSelection(selection(width));
	m_heightChoice->SetSelection(selection(height));
	static_cast<StagePanel*>(m_editPanel)->arrangeAllSprites(true);
	m_editPanel->Refresh();
}

wxSizer* ToolbarPanel::initLayout()
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
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::onRearrange));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Load All"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::onLoadLibraryList));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("Auto Arrange"));
		check->SetValue(Context::Instance()->auto_arrange);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::onChangeAutoArrange));
		sizer->Add(check);
	}
	sizer->AddSpacer(10);
	
	return sizer;
}

void ToolbarPanel::initSizeSettingPanel(wxSizer* topSizer)
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
		wxCommandEventHandler(ToolbarPanel::onChangeOutputImageSize));
	widthSizer->Add(m_widthChoice);
	sizer->Add(widthSizer);

	wxSizer* heightSizer = new wxBoxSizer(wxHORIZONTAL);
	heightSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Height: ")));
	m_heightChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, TOTLE_EDGE_TYPES, choices);
	m_heightChoice->SetSelection(3);
	Connect(m_heightChoice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
		wxCommandEventHandler(ToolbarPanel::onChangeOutputImageSize));
	heightSizer->Add(m_heightChoice);
	sizer->Add(heightSizer);

	topSizer->Add(sizer);
}

void ToolbarPanel::initSettingsPanel(wxSizer* topSizer)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Settings"));
	wxBoxSizer* settingsSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("padding: ")));

		wxSpinCtrl* padding = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 0, 100, 0);
		Connect(padding->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onChangePadding));
		sizer->Add(padding);

		settingsSizer->Add(sizer);
	}
	settingsSizer->AddSpacer(10);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("scale: ")));

		wxSpinCtrl* scale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 10, 200, 100);
		Connect(scale->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onChangeScale));
		sizer->Add(scale);

		settingsSizer->Add(sizer);
	}
	topSizer->Add(settingsSizer);
}

void ToolbarPanel::initFormatChoicePanel(wxSizer* topSizer)
{
	wxArrayString choices;
	choices.Add(wxT("png"));
	choices.Add(wxT("jpg"));
//	choices.Add(wxT("bmp"));	
	m_formatChoice = new wxRadioBox(this, wxID_ANY, wxT("Format"), 
		wxDefaultPosition, wxDefaultSize, choices, 2, wxRA_SPECIFY_COLS);
	topSizer->Add(m_formatChoice);
}

void ToolbarPanel::onChangeOutputImageSize(wxCommandEvent& event)
{
	Context::Instance()->width = wxVariant(m_widthChoice->GetString(m_widthChoice->GetSelection())).GetInteger();
	Context::Instance()->height = wxVariant(m_heightChoice->GetString(m_heightChoice->GetSelection())).GetInteger();
	static_cast<StagePanel*>(m_editPanel)->arrangeAllSprites(true);
	m_editPanel->Refresh();
}

void ToolbarPanel::onChangePadding(wxSpinEvent& event)
{
	Context::Instance()->padding = event.GetValue();
	static_cast<StagePanel*>(m_editPanel)->arrangeAllSprites(true);
	m_editPanel->Refresh();
}

void ToolbarPanel::onChangeScale(wxSpinEvent& event)
{
	float scale = Context::Instance()->scale = event.GetValue() * 0.01f;

	std::vector<d2d::ISprite*> sprites;
	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);
	stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->setScale(scale, scale);

	stage->arrangeAllSprites(true);
	m_editPanel->Refresh();
}

void ToolbarPanel::onRearrange(wxCommandEvent& event)
{
	static_cast<StagePanel*>(m_editPanel)->arrangeAllSprites(true);
	m_editPanel->Refresh();
}

void ToolbarPanel::onLoadLibraryList(wxCommandEvent& event)
{
	static_cast<StagePanel*>(m_editPanel)->loadFromLibrary();
	m_editPanel->Refresh();
}

void ToolbarPanel::onChangeAutoArrange(wxCommandEvent& event)
{
	Context::Instance()->auto_arrange = event.IsChecked();
}
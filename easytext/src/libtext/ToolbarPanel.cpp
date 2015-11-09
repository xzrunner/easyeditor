#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace etext
{

static const wxString HORI_ALIGN_LABELS[] = { 
	wxT("left"), wxT("right"), wxT("center"), wxT("auto") };
static const wxString VERT_ALIGN_LABELS[] = { 
	wxT("top"), wxT("bottom"), wxT("center"), wxT("auto") };

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage, Sprite* spr)
	: d2d::ToolbarPanel(parent, stage->GetStageImpl())
{
	spr->Retain();
	m_spr = spr;

	SetSizer(initLayout());	
}

ToolbarPanel::~ToolbarPanel()
{
	m_spr->Release();
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->AddSpacer(10);
	{
		int w, h;
		m_spr->GetSize(w, h);

		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "Size");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "width"), 0, wxLEFT | wxRIGHT, 5);

			m_width = new wxTextCtrl(this, wxID_ANY, d2d::StringTools::ToString(w),
				wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
			Connect(m_width->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
			csizer->Add(m_width);

			sizer->Add(csizer);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "height"), 0, wxLEFT | wxRIGHT, 5);

			m_height = new wxTextCtrl(this, wxID_ANY, d2d::StringTools::ToString(h),
				wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
			Connect(m_height->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
			csizer->Add(m_height);

			sizer->Add(csizer);
		}
		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(10);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "Font");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			const std::vector<std::pair<std::string, std::string> >& 
				fonts = d2d::Config::Instance()->GetFonts();
			wxArrayString choices;
			for (int i = 0, n = fonts.size(); i < n; ++i) {
				choices.push_back(fonts[i].first);
			}
			m_font = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
			m_font->SetSelection(0);
			Connect(m_font->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ToolbarPanel::OnChangeFont));
			sizer->Add(m_font);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "size"), 0, wxLEFT | wxRIGHT, 5);

			int size = m_spr->GetFontSize();
			m_font_size = new wxTextCtrl(this, wxID_ANY, d2d::StringTools::ToString(size),
				wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
			Connect(m_font_size->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeFontSize));
			csizer->Add(m_font_size);

			sizer->Add(csizer);
		}
		sizer->AddSpacer(5);
		{
			m_edge = new wxCheckBox(this, wxID_ANY, "edge");
			sizer->Add(m_edge);
		}
		sizer->AddSpacer(5);
		{
			wxButton* btn = new wxButton(this, wxID_ANY, "color");
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(ToolbarPanel::OnChangeColor));
			sizer->Add(btn);
		}
		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(10);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "Layout");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "hori"), 0, wxLEFT | wxRIGHT, 5);

			m_align_hori = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, HORI_ALIGN_LABELS);
			m_align_hori->SetSelection(0);
			Connect(m_align_hori->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(ToolbarPanel::OnChangeAlign));
			csizer->Add(m_align_hori);

			sizer->Add(csizer);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "vert"), 0, wxLEFT | wxRIGHT, 5);

			m_align_vert = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, VERT_ALIGN_LABELS);
			m_align_vert->SetSelection(0);
			Connect(m_align_vert->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(ToolbarPanel::OnChangeAlign));
			csizer->Add(m_align_vert);

			sizer->Add(csizer);
		}
		top_sizer->Add(sizer);
	}
	return top_sizer;
}

void ToolbarPanel::OnChangeSize(wxCommandEvent& event)
{	
	int w = d2d::StringTools::FromString<int>(m_width->GetValue().ToStdString());
	int h = d2d::StringTools::FromString<int>(m_height->GetValue().ToStdString());
	m_spr->SetSize(w, h);
}

void ToolbarPanel::OnChangeFont(wxCommandEvent& event)
{
	int font = m_font->GetSelection();
	m_spr->SetFont(font);
}

void ToolbarPanel::OnChangeFontSize(wxCommandEvent& event)
{
	int sz = d2d::StringTools::FromString<int>(m_font_size->GetValue().ToStdString());
	m_spr->SetFontSize(sz);
}

void ToolbarPanel::OnChangeColor(wxCommandEvent& event)
{
	d2d::RGBColorSettingDlg dlg(this, NULL, m_spr->GetColor());
	if (dlg.ShowModal()) {
		m_spr->SetColor(dlg.GetColor());
	}
}

void ToolbarPanel::OnChangeAlign(wxCommandEvent& event)
{
	int halign = m_align_hori->GetSelection();
	int valign = m_align_vert->GetSelection();
	m_spr->SetAlign(halign, valign);
}

}
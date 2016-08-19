#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "Sprite.h"

#include <ee/StringHelper.h>
#include <ee/Config.h>
#include <ee/RGBColorSettingDlg.h>

namespace etext
{

static const wxString HORI_ALIGN_LABELS[] = { 
	wxT("左"), wxT("右"), wxT("中"), wxT("自动") };
static const wxString VERT_ALIGN_LABELS[] = { 
	wxT("上"), wxT("下"), wxT("中"), wxT("自动") };

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage, Sprite* spr)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	spr->AddReference();
	m_spr = spr;

	SetSizer(InitLayout());	
}

ToolbarPanel::~ToolbarPanel()
{
	m_spr->RemoveReference();
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->AddSpacer(20);
	InitSizeLayout(top_sizer);
	top_sizer->AddSpacer(20);
	InitFontLayout(top_sizer);
	top_sizer->AddSpacer(20);
	InitLayoutLayout(top_sizer);
	return top_sizer;
}

void ToolbarPanel::InitSizeLayout(wxSizer* top_sizer)
{
	const s2::Textbox& tb = m_spr->GetTextbox();

	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "Size");
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "宽"), 0, wxLEFT | wxRIGHT, 5);

		m_width = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(tb.width),
			wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		Connect(m_width->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
		csizer->Add(m_width);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "高"), 0, wxLEFT | wxRIGHT, 5);

		m_height = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(tb.height),
			wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		Connect(m_height->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
		csizer->Add(m_height);

		sizer->Add(csizer);
	}
	top_sizer->Add(sizer);
}

void ToolbarPanel::InitFontLayout(wxSizer* top_sizer)
{
	const s2::Textbox& tb = m_spr->GetTextbox();

	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "Font");
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		const std::vector<std::pair<std::string, std::string> >& 
			fonts = ee::Config::Instance()->GetFonts();
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
		csizer->Add(new wxStaticText(this, wxID_ANY, "字号"), 0, wxLEFT | wxRIGHT, 5);

		m_font_size = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(tb.font_size),
			wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		Connect(m_font_size->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeFontSize));
		csizer->Add(m_font_size);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "颜色");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnChangeFontColor));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		m_edge = new wxCheckBox(this, wxID_ANY, "描边");
		m_edge->SetValue(tb.has_edge);
		Connect(m_edge->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnChangeEdge));
		sizer->Add(m_edge);
	}
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "描边");
		wxSizer* e_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "宽度"), 0, wxLEFT | wxRIGHT, 5);

			m_edge_size = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(tb.edge_size),
				wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
			Connect(m_edge_size->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeEdgeSize));
			csizer->Add(m_edge_size);

			e_sizer->Add(csizer);
		}
		e_sizer->AddSpacer(5);
		{
			wxButton* btn = new wxButton(this, wxID_ANY, "颜色");
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(ToolbarPanel::OnChangeEdgeColor));
			e_sizer->Add(btn);
		}
		sizer->Add(e_sizer);
	}
	top_sizer->Add(sizer);
}

void ToolbarPanel::InitLayoutLayout(wxSizer* top_sizer)
{
	const s2::Textbox& tb = m_spr->GetTextbox();

	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "Layout");
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "水平对齐"), 0, wxLEFT | wxRIGHT, 5);

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
		csizer->Add(new wxStaticText(this, wxID_ANY, "竖直对齐"), 0, wxLEFT | wxRIGHT, 5);

		m_align_vert = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, VERT_ALIGN_LABELS);
		m_align_vert->SetSelection(0);
		Connect(m_align_vert->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnChangeAlign));
		csizer->Add(m_align_vert);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(10);

	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "字间距"), 0, wxLEFT | wxRIGHT, 5);

		m_space_h = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(tb.space_hori),
			wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		Connect(m_space_h->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSpace));
		csizer->Add(m_space_h);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "行间距"), 0, wxLEFT | wxRIGHT, 5);

		m_space_v = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(tb.space_vert),
			wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		Connect(m_space_v->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSpace));
		csizer->Add(m_space_v);

		sizer->Add(csizer);
	}
	top_sizer->Add(sizer);
}

void ToolbarPanel::OnChangeSize(wxCommandEvent& event)
{	
	s2::Textbox& tb = m_spr->GetTextbox();
	tb.width = ee::StringHelper::FromString<int>(m_width->GetValue().ToStdString());
	tb.height = ee::StringHelper::FromString<int>(m_height->GetValue().ToStdString());
	m_spr->UpdateBounding();
}

void ToolbarPanel::OnChangeFont(wxCommandEvent& event)
{
	s2::Textbox& tb = m_spr->GetTextbox();
	tb.font_type = m_font->GetSelection();
}

void ToolbarPanel::OnChangeFontSize(wxCommandEvent& event)
{
	s2::Textbox& tb = m_spr->GetTextbox();
	tb.font_size = ee::StringHelper::FromString<int>(m_font_size->GetValue().ToStdString());
}

void ToolbarPanel::OnChangeFontColor(wxCommandEvent& event)
{
	s2::Textbox& tb = m_spr->GetTextbox();
	ee::RGBColorSettingDlg dlg(this, NULL, tb.font_color);
	if (dlg.ShowModal() == wxID_OK) {
		tb.font_color = dlg.GetColor();
	}
}

void ToolbarPanel::OnChangeEdge(wxCommandEvent& event)
{
	s2::Textbox& tb = m_spr->GetTextbox();
	tb.has_edge = event.IsChecked();
}

void ToolbarPanel::OnChangeEdgeSize(wxCommandEvent& event)
{
	s2::Textbox& tb = m_spr->GetTextbox();
	tb.edge_size = ee::StringHelper::FromString<float>(m_edge_size->GetValue().ToStdString());
}

void ToolbarPanel::OnChangeEdgeColor(wxCommandEvent& event)
{
	s2::Textbox& tb = m_spr->GetTextbox();
	ee::RGBColorSettingDlg dlg(this, NULL, tb.edge_color);
	if (dlg.ShowModal() == wxID_OK) {
		tb.edge_color = dlg.GetColor();
	}
}

void ToolbarPanel::OnChangeAlign(wxCommandEvent& event)
{
	s2::Textbox& tb = m_spr->GetTextbox();
	tb.align_hori = s2::Textbox::HoriAlign(m_align_hori->GetSelection());
	tb.align_vert = s2::Textbox::VertAlign(m_align_vert->GetSelection());
}

void ToolbarPanel::OnChangeSpace(wxCommandEvent& event)
{
	s2::Textbox& tb = m_spr->GetTextbox();
	tb.space_hori = ee::StringHelper::FromString<float>(m_space_h->GetValue().ToStdString());
	tb.space_vert = ee::StringHelper::FromString<float>(m_space_v->GetValue().ToStdString());
}

}
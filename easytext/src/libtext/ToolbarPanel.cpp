#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace etext
{

static const wxString HORI_ALIGN_LABELS[] = { 
	wxT("左"), wxT("右"), wxT("中"), wxT("自动") };
static const wxString VERT_ALIGN_LABELS[] = { 
	wxT("上"), wxT("下"), wxT("中"), wxT("自动") };

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
	int w, h;
	m_spr->GetSize(w, h);

	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "Size");
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "宽"), 0, wxLEFT | wxRIGHT, 5);

		m_width = new wxTextCtrl(this, wxID_ANY, d2d::StringHelper::ToString(w),
			wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		Connect(m_width->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
		csizer->Add(m_width);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "高"), 0, wxLEFT | wxRIGHT, 5);

		m_height = new wxTextCtrl(this, wxID_ANY, d2d::StringHelper::ToString(h),
			wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		Connect(m_height->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
		csizer->Add(m_height);

		sizer->Add(csizer);
	}
	top_sizer->Add(sizer);
}

void ToolbarPanel::InitFontLayout(wxSizer* top_sizer)
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
		csizer->Add(new wxStaticText(this, wxID_ANY, "字号"), 0, wxLEFT | wxRIGHT, 5);

		int size = m_spr->GetFontSize();
		m_font_size = new wxTextCtrl(this, wxID_ANY, d2d::StringHelper::ToString(size),
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
		m_edge->SetValue(m_spr->GetEdge());
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

			float size = m_spr->GetEdgeSize();
			m_edge_size = new wxTextCtrl(this, wxID_ANY, d2d::StringHelper::ToString(size),
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

	float space_h, space_v;
	m_spr->GetSpace(space_h, space_v);		
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "字间距"), 0, wxLEFT | wxRIGHT, 5);

		m_space_h = new wxTextCtrl(this, wxID_ANY, d2d::StringHelper::ToString(space_h),
			wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		Connect(m_space_h->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSpace));
		csizer->Add(m_space_h);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "行间距"), 0, wxLEFT | wxRIGHT, 5);

		m_space_v = new wxTextCtrl(this, wxID_ANY, d2d::StringHelper::ToString(space_v),
			wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		Connect(m_space_v->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSpace));
		csizer->Add(m_space_v);

		sizer->Add(csizer);
	}
	top_sizer->Add(sizer);
}

void ToolbarPanel::OnChangeSize(wxCommandEvent& event)
{	
	int w = d2d::StringHelper::FromString<int>(m_width->GetValue().ToStdString());
	int h = d2d::StringHelper::FromString<int>(m_height->GetValue().ToStdString());
	m_spr->SetSize(w, h);
}

void ToolbarPanel::OnChangeFont(wxCommandEvent& event)
{
	int font = m_font->GetSelection();
	m_spr->SetFont(font);
}

void ToolbarPanel::OnChangeFontSize(wxCommandEvent& event)
{
	int sz = d2d::StringHelper::FromString<int>(m_font_size->GetValue().ToStdString());
	m_spr->SetFontSize(sz);
}

void ToolbarPanel::OnChangeFontColor(wxCommandEvent& event)
{
	d2d::RGBColorSettingDlg dlg(this, NULL, m_spr->GetFontColor());
	if (dlg.ShowModal() == wxID_OK) {
		m_spr->SetFontColor(dlg.GetColor());
	}
}

void ToolbarPanel::OnChangeEdge(wxCommandEvent& event)
{
	m_spr->SetEdge(event.IsChecked());
}

void ToolbarPanel::OnChangeEdgeSize(wxCommandEvent& event)
{
	int sz = d2d::StringHelper::FromString<float>(m_edge_size->GetValue().ToStdString());
	m_spr->SetEdgeSize(sz);
}

void ToolbarPanel::OnChangeEdgeColor(wxCommandEvent& event)
{
	d2d::RGBColorSettingDlg dlg(this, NULL, m_spr->GetEdgeColor());
	if (dlg.ShowModal() == wxID_OK) {
		m_spr->SetEdgeColor(dlg.GetColor());
	}
}

void ToolbarPanel::OnChangeAlign(wxCommandEvent& event)
{
	int halign = m_align_hori->GetSelection();
	int valign = m_align_vert->GetSelection();
	m_spr->SetAlign(halign, valign);
}

void ToolbarPanel::OnChangeSpace(wxCommandEvent& event)
{
	float hspace = d2d::StringHelper::FromString<float>(m_space_h->GetValue().ToStdString());
	float vspace = d2d::StringHelper::FromString<float>(m_space_v->GetValue().ToStdString());
	m_spr->SetSpace(hspace, vspace);
}

}
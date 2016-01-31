
#include "SettingViewDlg.h"

#include <wx/spinctrl.h>

using namespace emodeling;

SettingViewDlg::SettingViewDlg(wxWindow* parent, ee::StageCanvas* canvas)
	: wxDialog(parent, wxID_ANY, wxT("Display Setting"))
	, m_canvas(canvas)
	, m_ctlPointSize(NULL)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->AddSpacer(20);
	sizer->Add(initDisplayTypePanel(), 0, wxCENTRE/* | wxALL, 20*/);
	sizer->AddSpacer(20);
	sizer->Add(initStylePanel(), 0, wxCENTRE/* | wxALL, 20*/);
	SetSizer(sizer);

	sizer->Layout();
	Refresh(true);
}

wxSizer* SettingViewDlg::initDisplayTypePanel()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString choices;
	choices.Add(wxT("physics+image"));
	choices.Add(wxT("physics"));
	choices.Add(wxT("image"));

	wxRadioBox* drawChoice = new wxRadioBox(this, wxID_ANY, wxT("display type"), 
		wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
	Connect(drawChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, 
		wxCommandEventHandler(SettingViewDlg::onChangeDisplayType));
	drawChoice->SetSelection(ee::SettingData::draw_type);

	sizer->Add(drawChoice);

	return sizer;
}

wxSizer* SettingViewDlg::initStylePanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("display style"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("polyline"));
		wxBoxSizer* lineSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("node size:")));

			m_ctlPointSize = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
				wxSP_ARROW_KEYS, 0, 30, ee::SettingData::ctl_pos_sz);
			Connect(m_ctlPointSize->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(SettingViewDlg::onChangeStyle));
			sizer->Add(m_ctlPointSize);

			lineSizer->Add(sizer);
		}
		topSizer->Add(lineSizer);
	}
	topSizer->AddSpacer(10);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("polygon"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxCheckBox* trisCheck = new wxCheckBox(this, wxID_ANY, wxT("triangle edge"));
			trisCheck->SetValue(ee::SettingData::draw_tris_edge);
			Connect(trisCheck->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingViewDlg::onChangeDisplayTriangles));
			sizer->Add(trisCheck);
		}
		topSizer->AddSpacer(5);
		{
			wxCheckBox* boundCheck = new wxCheckBox(this, wxID_ANY, wxT("bounding"));
			boundCheck->SetValue(ee::SettingData::draw_poly_bound);
			Connect(boundCheck->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingViewDlg::onChangeDisplayPolygonBound));
			sizer->Add(boundCheck);
		}
		topSizer->Add(sizer);
	}
	return topSizer;
}

void SettingViewDlg::onChangeDisplayType(wxCommandEvent& event)
{
	ee::SettingData::draw_type = static_cast<ee::SettingData::DrawType>(event.GetSelection());

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SettingViewDlg::onChangeStyle(wxSpinEvent& event)
{
	ee::SettingData::ctl_pos_sz = m_ctlPointSize->GetValue();
	if (m_canvas) m_canvas->Refresh(true);
}

void SettingViewDlg::onChangeDisplayTriangles(wxCommandEvent& event)
{
	ee::SettingData::draw_tris_edge = event.IsChecked();
	if (m_canvas) m_canvas->Refresh(true);
}

void SettingViewDlg::onChangeDisplayPolygonBound(wxCommandEvent& event)
{
	ee::SettingData::draw_poly_bound = event.IsChecked();
	if (m_canvas) m_canvas->Refresh(true);
}
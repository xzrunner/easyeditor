#include "ShapeCompPanel.h"
#include "mt_config.h"

#include <ee/SliderCtrlOne.h>

#include <mt_2d.h>

#include <wx/colourdata.h>
#include <wx/colordlg.h>

namespace etrail
{

ShapeCompPanel::ShapeCompPanel(wxWindow* parent, t2d_symbol* pc, ToolbarPanel* toolbar)
	: ComponentPanel(parent, pc, toolbar)
{
	ComponentPanel::InitLayout();
	SetBtnColor();
}

void ShapeCompPanel::SetValue(int key, const ee::UICallback::Data& data)
{
	switch (key)
	{
	case MT_LINE_WIDTH:
		m_pc->mode.A.size = data.val0;
		break;
	case MT_ACUITY:
		m_pc->mode.A.acuity = data.val0 * 0.01f;
		break;
	}
}

void ShapeCompPanel::GetValue(int key, ee::UICallback::Data& data)
{
	switch (key)
	{
	case MT_LINE_WIDTH:
		data.val0 = m_pc->mode.A.size;
		break;
	case MT_ACUITY:
		data.val0 = m_pc->mode.A.acuity * 100;
		break;
	}
}

void ShapeCompPanel::Load(const Json::Value& val)
{
	ComponentPanel::Load(val);

	Json::Value col_val = val["color"];
	m_pc->color.r = col_val["r"].asInt();
	m_pc->color.g = col_val["g"].asInt();
	m_pc->color.b = col_val["b"].asInt();
	m_pc->color.a = col_val["a"].asInt();
	SetBtnColor();
}

void ShapeCompPanel::Store(Json::Value& val) const
{
	ComponentPanel::Store(val);

	Json::Value col_val;
	col_val["r"] = m_pc->color.r;
	col_val["g"] = m_pc->color.g;
	col_val["b"] = m_pc->color.b;
	col_val["a"] = m_pc->color.a;
	val["color"] = col_val;
}

void ShapeCompPanel::InitLayout(wxSizer* top_sizer)
{
	// top
	{
		wxSizer* hori_sizer = new wxBoxSizer(wxHORIZONTAL);
		// Delete
		{
			wxButton* btn_del = new wxButton(this, wxID_ANY, "É¾³ý");
			Connect(btn_del->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ShapeCompPanel::OnDelete));
			hori_sizer->Add(btn_del);
		}
		hori_sizer->AddSpacer(20);
		// Color
		{
			wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "ÑÕÉ«");
			wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

			m_col_btn = new wxButton(this, wxID_ANY);
			Connect(m_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ShapeCompPanel::OnSetColor));
			sizer->Add(m_col_btn);

			hori_sizer->Add(sizer);
		}
		top_sizer->Add(hori_sizer);
	}

	// Line width
	ee::SliderCtrlOne* s_width = new ee::SliderCtrlOne(this, "Ïß¿í", "linewidth", this, MT_LINE_WIDTH, 
		ee::SliderItem("", "", 10, 5, 30));
	top_sizer->Add(s_width);
	m_sliders.push_back(s_width);

	// Acuity
	ee::SliderCtrlOne* s_acuity = new ee::SliderCtrlOne(this, "¼âÈñ", "acuity", this, MT_ACUITY, 
		ee::SliderItem("", "", 90, 0, 100));
	top_sizer->Add(s_acuity);
	m_sliders.push_back(s_acuity);
}

void ShapeCompPanel::OnSetColor(wxCommandEvent& event)
{
	wxColourData data;
	data.SetColour(wxColour(m_pc->color.r, m_pc->color.g, m_pc->color.b));
	wxColourDialog dlg(m_parent, &data);

	dlg.SetTitle(wxT("Set Color"));

	wxPoint pos = wxGetMousePosition();
	pos.x -= 400;
	dlg.SetPosition(pos);

	if (dlg.ShowModal() == wxID_OK)
	{
		const wxColor& col = dlg.GetColourData().GetColour();
		m_pc->color.r = col.Red();
		m_pc->color.g = col.Green();
		m_pc->color.b = col.Blue();
		m_pc->color.a = col.Alpha();
		SetBtnColor();
	}
}

void ShapeCompPanel::SetBtnColor()
{
	wxColor wx_col(m_pc->color.r, m_pc->color.g, m_pc->color.b, m_pc->color.a);
	m_col_btn->SetBackgroundColour(wx_col);
}

}
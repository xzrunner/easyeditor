#include "ShapeCompPanel.h"
#include "mt_config.h"

#include <ee/SliderCtrlOne.h>
#include <ee/SliderCtrlTwo.h>
#include <ee/JsonSerializer.h>

#include <mt_2d.h>

namespace etrail
{

ShapeCompPanel::ShapeCompPanel(wxWindow* parent, t2d_symbol* pc, ToolbarPanel* toolbar)
	: ComponentPanel(parent, pc, toolbar)
{
	ComponentPanel::InitLayout();
	UpdateBtnColor();
}

void ShapeCompPanel::SetValue(int key, const ee::UICallback::Data& data)
{
	ComponentPanel::SetValue(key, data);

	switch (key)
	{
	case MT_LINE_WIDTH:
		m_pc->mode.B.size = data.val0;
		break;
	case MT_ACUITY:
		m_pc->mode.B.acuity = data.val0 * 0.01f;
		break;
	}
}

void ShapeCompPanel::GetValue(int key, ee::UICallback::Data& data)
{
	ComponentPanel::GetValue(key, data);

	switch (key)
	{
	case MT_LINE_WIDTH:
		data.val0 = m_pc->mode.B.size;
		break;
	case MT_ACUITY:
		data.val0 = m_pc->mode.B.acuity * 100;
		break;
	}
}

void ShapeCompPanel::Load(const Json::Value& val, const std::string& dir)
{
	ComponentPanel::Load(val, dir);

	ee::JsonSerializer::Load(val["color_begin"], m_pc->col_begin.rgba);
	ee::JsonSerializer::Load(val["color_end"], m_pc->col_end.rgba);

	UpdateBtnColor();
}

void ShapeCompPanel::Store(Json::Value& val, const std::string& dir) const
{
	ComponentPanel::Store(val, dir);

	ee::JsonSerializer::Store(m_pc->col_begin.rgba, val["color_begin"]);
	ee::JsonSerializer::Store(m_pc->col_end.rgba, val["color_end"]);
}

void ShapeCompPanel::InitLayout(wxSizer* top_sizer)
{
	// top
	{
		wxSizer* hori_sizer = new wxBoxSizer(wxHORIZONTAL);
		// Delete
		{
			wxButton* btn_del = new wxButton(this, wxID_ANY, "删除");
			Connect(btn_del->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ShapeCompPanel::OnDelete));
			hori_sizer->Add(btn_del);
		}
		hori_sizer->AddSpacer(20);
		// Color
		{
			wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "起始颜色");
			wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

			m_begin_col_btn = new wxButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, 20));
			Connect(m_begin_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ShapeCompPanel::OnSetBeginColor));
			sizer->Add(m_begin_col_btn);

			hori_sizer->Add(sizer);
		}
		{
			wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "结束颜色");
			wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

			m_end_col_btn = new wxButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, 20));
			Connect(m_end_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ShapeCompPanel::OnSetEndColor));
			sizer->Add(m_end_col_btn);

			hori_sizer->Add(sizer);
		}

		top_sizer->Add(hori_sizer);
	}

	// Line width
	ee::SliderCtrlOne* s_width = new ee::SliderCtrlOne(this, "线宽", "linewidth", this, MT_LINE_WIDTH, 
		ee::SliderItem("", "", 10, 5, 30));
	top_sizer->Add(s_width);
	m_sliders.push_back(s_width);

	// Acuity
	ee::SliderCtrlOne* s_acuity = new ee::SliderCtrlOne(this, "尖锐", "acuity", this, MT_ACUITY, 
		ee::SliderItem("", "", 90, 0, 100));
	top_sizer->Add(s_acuity);
	m_sliders.push_back(s_acuity);

	// Alpha
	ee::SliderCtrlTwo* s_alpha = new ee::SliderCtrlTwo(this, "透明度", "alpha", this, MT_ALPHA, 
		ee::SliderItem("起始", "begin", 255, 0, 255), ee::SliderItem("结束", "end", 255, 0, 255));
	top_sizer->Add(s_alpha);
//	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_alpha);

}

void ShapeCompPanel::UpdateBtnColor()
{
	wxColor col_begin(m_pc->col_begin.r, m_pc->col_begin.g, m_pc->col_begin.b, m_pc->col_begin.a);
	m_begin_col_btn->SetBackgroundColour(col_begin);

	wxColor col_end(m_pc->col_end.r, m_pc->col_end.g, m_pc->col_end.b, m_pc->col_end.a);
	m_end_col_btn->SetBackgroundColour(col_end);
}

void ShapeCompPanel::OnSetBeginColor(wxCommandEvent& event)
{
	ChangeColor(m_pc->col_begin.rgba);
}

void ShapeCompPanel::OnSetEndColor(wxCommandEvent& event)
{
	ChangeColor(m_pc->col_end.rgba);
}

}
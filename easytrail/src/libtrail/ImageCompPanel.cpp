#include "ImageCompPanel.h"
#include "mt_config.h"
#include "ToolbarPanel.h"

#include <ee/ImagePanel.h>
#include <ee/SliderCtrlTwo.h>
#include <ee/Symbol.h>
#include <ee/FileType.h>
#include <ee/FileHelper.h>
#include <ee/JsonSerializer.h>

#include <mt_2d.h>

namespace etrail
{

ImageCompPanel::ImageCompPanel(wxWindow* parent, t2d_symbol* pc, ToolbarPanel* toolbar)
	: ComponentPanel(parent, pc, toolbar)
{
	ComponentPanel::InitLayout();
	UpdateBtnColor();
}

void ImageCompPanel::SetValue(int key, const ee::UICallback::Data& data)
{
	ComponentPanel::SetValue(key, data);

	switch (key)
	{
	case MT_SCALE:
		m_pc->mode.A.scale_begin = data.val0 * 0.01f;
		m_pc->mode.A.scale_end = data.val1 * 0.01f;
		break;
	}
}

void ImageCompPanel::GetValue(int key, ee::UICallback::Data& data)
{
	ComponentPanel::GetValue(key, data);

	switch (key)
	{
	case MT_SCALE:
		data.val0 = m_pc->mode.A.scale_begin * 100;
		data.val1 = m_pc->mode.A.scale_end * 100;
		break;
	}
}

void ImageCompPanel::Load(const Json::Value& val, const std::string& dir)
{
	ComponentPanel::Load(val, dir);

	ee::JsonSerializer::Load(val["mul_col_begin"], m_pc->col_begin.rgba);
	ee::JsonSerializer::Load(val["mul_col_end"], m_pc->col_end.rgba);
	ee::JsonSerializer::Load(val["add_col_begin"], m_pc->mode.A.add_col_begin.rgba);
	ee::JsonSerializer::Load(val["add_col_end"], m_pc->mode.A.add_col_end.rgba);

	UpdateBtnColor();
}

void ImageCompPanel::Store(Json::Value& val, const std::string& dir) const
{
	ComponentPanel::Store(val, dir);

	ee::JsonSerializer::Store(m_pc->col_begin.rgba, val["mul_col_begin"]);
	ee::JsonSerializer::Store(m_pc->col_end.rgba, val["mul_col_end"]);
	ee::JsonSerializer::Store(m_pc->mode.A.add_col_begin.rgba, val["add_col_begin"]);
	ee::JsonSerializer::Store(m_pc->mode.A.add_col_end.rgba, val["add_col_end"]);

	s2::Symbol* sym = static_cast<ee::Symbol*>(m_pc->mode.A.ud);
	val["filepath"] = ee::FileHelper::GetRelativePath(dir, dynamic_cast<ee::Symbol*>(sym)->GetFilepath());
}

void ImageCompPanel::InitLayout(wxSizer* top_sizer)
{
	// top
	{
		wxSizer* hori_sizer = new wxBoxSizer(wxHORIZONTAL);
		// Left
		{
			wxSizer* vert_sizer = new wxBoxSizer(wxVERTICAL);
			// Del
			{
				wxButton* btn = new wxButton(this, wxID_ANY, "删除");
				Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImageCompPanel::OnDelete));
				vert_sizer->Add(btn);
			}
			hori_sizer->Add(vert_sizer);
		}
		hori_sizer->AddSpacer(20);
		// right
		{
			std::string filepath = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(m_pc->mode.A.ud))->GetFilepath();
			if (ee::FileType::IsType(filepath, ee::FileType::e_image)) {
				ee::ImagePanel* panel = new ee::ImagePanel(this, filepath, 100);
				hori_sizer->Add(panel);
			}
		}
		top_sizer->Add(hori_sizer);
	}
	// Mul Color
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "乘色"); 
		wxSizer* hori_sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		hori_sizer->Add(new wxStaticText(this, wxID_ANY, "起始"));
		hori_sizer->AddSpacer(5);
		m_begin_mul_col_btn = new wxButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, 20));
		Connect(m_begin_mul_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImageCompPanel::OnSetBeginMulCol));
		hori_sizer->Add(m_begin_mul_col_btn);

		hori_sizer->AddSpacer(20);

		hori_sizer->Add(new wxStaticText(this, wxID_ANY, "结束"));
		hori_sizer->AddSpacer(5);
		m_end_mul_col_btn = new wxButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, 20));
		Connect(m_end_mul_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImageCompPanel::OnSetEndMulCol));
		hori_sizer->Add(m_end_mul_col_btn);

		top_sizer->Add(hori_sizer);
	}
	// Add Color
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "加色"); 
		wxSizer* hori_sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		hori_sizer->Add(new wxStaticText(this, wxID_ANY, "起始"));
		hori_sizer->AddSpacer(5);
		m_begin_add_col_btn = new wxButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, 20));
		Connect(m_begin_add_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImageCompPanel::OnSetBeginAddCol));
		hori_sizer->Add(m_begin_add_col_btn);

		hori_sizer->AddSpacer(20);

		hori_sizer->Add(new wxStaticText(this, wxID_ANY, "结束"));
		hori_sizer->AddSpacer(5);
		m_end_add_col_btn = new wxButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, 20));
		Connect(m_end_add_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImageCompPanel::OnSetEndAddCol));
		hori_sizer->Add(m_end_add_col_btn);

		top_sizer->Add(hori_sizer);
	}
	// Scale
	ee::SliderCtrlTwo* s_scale = new ee::SliderCtrlTwo(this, "缩放", "scale", this, MT_SCALE, 
		ee::SliderItem("起始", "start", 100, 0, 2000), ee::SliderItem("结束", "end", 100, 0, 2000));
	top_sizer->Add(s_scale);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_scale);
}

std::string ImageCompPanel::GetTitle() const
{
	std::string name = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(m_pc->mode.A.ud))->GetFilepath();
	name = ee::FileHelper::GetFilename(name);
	return name;
}

void ImageCompPanel::UpdateBtnColor()
{
	wxColor mul_col_begin(m_pc->col_begin.r, m_pc->col_begin.g, m_pc->col_begin.b, m_pc->col_begin.a);
	m_begin_mul_col_btn->SetBackgroundColour(mul_col_begin);

	wxColor mul_col_end(m_pc->col_end.r, m_pc->col_end.g, m_pc->col_end.b, m_pc->col_end.a);
	m_end_mul_col_btn->SetBackgroundColour(mul_col_end);

	wxColor add_col_begin(m_pc->mode.A.add_col_begin.r, m_pc->mode.A.add_col_begin.g, m_pc->mode.A.add_col_begin.b, m_pc->mode.A.add_col_begin.a);
	m_begin_add_col_btn->SetBackgroundColour(add_col_begin);

	wxColor add_col_end(m_pc->mode.A.add_col_end.r, m_pc->mode.A.add_col_end.g, m_pc->mode.A.add_col_end.b, m_pc->mode.A.add_col_end.a);
	m_end_add_col_btn->SetBackgroundColour(add_col_end);
}

void ImageCompPanel::OnSetBeginMulCol(wxCommandEvent& event)
{
	ChangeColor(m_pc->col_begin.rgba);
}

void ImageCompPanel::OnSetEndMulCol(wxCommandEvent& event)
{
	ChangeColor(m_pc->col_end.rgba);
}

void ImageCompPanel::OnSetBeginAddCol(wxCommandEvent& event)
{
	ChangeColor(m_pc->mode.A.add_col_begin.rgba);
}

void ImageCompPanel::OnSetEndAddCol(wxCommandEvent& event)
{
	ChangeColor(m_pc->mode.A.add_col_end.rgba);
}

}
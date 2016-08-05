#include "ImageCompPanel.h"
#include "mt_config.h"
#include "ToolbarPanel.h"

#include <ee/ImagePanel.h>
#include <ee/SliderCtrlTwo.h>
#include <ee/Symbol.h>
#include <ee/FileType.h>
#include <ee/FileHelper.h>

#include <mt_2d.h>

namespace etrail
{

ImageCompPanel::ImageCompPanel(wxWindow* parent, t2d_symbol* pc, ToolbarPanel* toolbar)
	: ComponentPanel(parent, pc, toolbar)
{
	ComponentPanel::InitLayout();
}

void ImageCompPanel::SetValue(int key, const ee::UICallback::Data& data)
{

}

void ImageCompPanel::GetValue(int key, ee::UICallback::Data& data)
{

}

void ImageCompPanel::Load(const Json::Value& val)
{
	ComponentPanel::Load(val);
}

void ImageCompPanel::Store(Json::Value& val) const
{
	ComponentPanel::Store(val);
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
			std::string filepath = static_cast<ee::Symbol*>(m_pc->mode.B.ud)->GetFilepath();
			if (ee::FileType::IsType(filepath, ee::FileType::e_image)) {
				ee::ImagePanel* panel = new ee::ImagePanel(this, filepath, 100);
				hori_sizer->Add(panel);
			}
		}
		top_sizer->Add(hori_sizer);
	}
	// Scale
	ee::SliderCtrlTwo* s_scale = new ee::SliderCtrlTwo(this, "缩放", "scale", this, MT_SCALE, 
		ee::SliderItem("起始", "start", 100, 0, 2000), ee::SliderItem("结束", "end", 100, 0, 2000));
	top_sizer->Add(s_scale);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_scale);
	// Alpha
	ee::SliderCtrlTwo* s_alpha = new ee::SliderCtrlTwo(this, "透明度", "alpha", this, MT_ALPHA, 
		ee::SliderItem("起始", "start", 100, 0, 100), ee::SliderItem("结束", "end", 100, 0, 100));
	top_sizer->Add(s_alpha);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_alpha);
}

std::string ImageCompPanel::GetTitle() const
{
	std::string name = static_cast<ee::Symbol*>(m_pc->mode.B.ud)->GetFilepath();
	name = ee::FileHelper::GetFilename(name);
	return name;
}

void ImageCompPanel::OnDelete(wxCommandEvent& event)
{
	m_toolbar->OnDelChild(this);
}

}
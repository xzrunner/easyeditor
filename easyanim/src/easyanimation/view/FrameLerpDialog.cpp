#include "FrameLerpDialog.h"
#include "dataset/KeyFrame.h"

#include <sprite2/LerpSpiral.h>
#include <sprite2/AnimLerp.h>
#include <sm_const.h>

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>

namespace eanim
{

FrameLerpDialog::FrameLerpDialog(wxWindow* parent, KeyFrame* frame)
	: wxDialog(parent, wxID_ANY, "Lerp")
	, m_frame(frame)
{
	InitLayout();
}

void FrameLerpDialog::Store()
{
	if (m_pos_spiral->IsChecked())
	{
		float start = m_spiral_angle_begin->GetValue() * SM_DEG_TO_RAD,
			  end = m_spiral_angle_end->GetValue() * SM_DEG_TO_RAD;
		float scale = m_spiral_scale->GetValue() * 0.01f;
		s2::LerpSpiral* lerp = new s2::LerpSpiral(start, end, scale);
		m_frame->SetLerp(s2::AnimLerp::SPR_POS, lerp);
	}
	else
	{
		m_frame->SetLerp(s2::AnimLerp::SPR_POS, NULL);
	}
}

void FrameLerpDialog::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	// todo

	bool pos_spiral = false;
	int sprial_begin = 0, sprial_end = 0;
	int sprial_scale = 100;

	const std::vector<std::pair<int, s2::ILerp*> >& lerps = m_frame->GetLerps();
	for (int i = 0, n = lerps.size(); i < n; ++i) 
	{
		if (lerps[i].first == s2::AnimLerp::SPR_POS) 
		{
			pos_spiral = true;
			s2::LerpSpiral* lerp = static_cast<s2::LerpSpiral*>(lerps[i].second);
			float begin, end;
			lerp->GetAngle(begin, end);
			sprial_begin = begin * SM_RAD_TO_DEG;
			sprial_end = end * SM_RAD_TO_DEG;
			sprial_scale = lerp->GetScale() * 100;
			break;
		}
	}

	m_pos_spiral = new wxCheckBox(this, wxID_ANY, ("位置spiral插值"));
	m_pos_spiral->SetValue(pos_spiral);
	top_sizer->Add(m_pos_spiral);

	top_sizer->AddSpacer(10);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, "起始角度"));	

		m_spiral_angle_begin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 360, sprial_begin);
		sizer->Add(m_spiral_angle_begin);

		top_sizer->Add(sizer);
	}
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, "结束角度"));	

		m_spiral_angle_end = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 3600, sprial_end);
		sizer->Add(m_spiral_angle_end);

		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(5);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, "缩放"));

		m_spiral_scale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, sprial_scale);
		sizer->Add(m_spiral_scale);

		top_sizer->Add(sizer);

	}

	//////////////////////////////////////////////////////////////////////////

	{
		SetEscapeId(wxID_CANCEL);
		top_sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxCENTER);
	}

	SetSizer(top_sizer);
	
	top_sizer->Layout();
	Refresh(true);
}

}
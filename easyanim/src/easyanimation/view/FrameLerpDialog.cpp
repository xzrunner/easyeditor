#include "FrameLerpDialog.h"
#include "dataset/KeyFrame.h"

#include <sprite2/LerpCircle.h>
#include <sprite2/LerpSpiral.h>
#include <sprite2/LerpWiggle.h>
#include <sprite2/LerpEase.h>
#include <sprite2/AnimLerp.h>
#include <sm_const.h>

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>

namespace eanim
{

FrameLerpDialog::FrameLerpDialog(wxWindow* parent, KeyFrame* frame)
	: wxDialog(parent, wxID_ANY, "Lerp", wxDefaultPosition, wxSize(300, 400))
	, m_frame(frame)
{
	InitLayout();
}

void FrameLerpDialog::Store()
{
	if (m_pos_circle->IsChecked()) 
	{
		float scale = m_circle_scale->GetValue() * 0.01f;
		auto lerp = std::make_unique<s2::LerpCircle>(scale);
		m_frame->SetLerp(s2::AnimLerp::SPR_POS, std::move(lerp));
	}
	else if (m_pos_spiral->IsChecked())
	{
		float start = m_spiral_angle_begin->GetValue() * SM_DEG_TO_RAD,
			  end = m_spiral_angle_end->GetValue() * SM_DEG_TO_RAD;
		float scale = m_spiral_scale->GetValue() * 0.01f;
		auto lerp = std::make_unique<s2::LerpSpiral>(start, end, scale);
		m_frame->SetLerp(s2::AnimLerp::SPR_POS, std::move(lerp));
	}
	else if (m_pos_wiggle->IsChecked())
	{
		float freq = m_wiggle_freq->GetValue();
		float amp = m_wiggle_amp->GetValue();
		auto lerp = std::make_unique<s2::LerpWiggle>(freq, amp);
		m_frame->SetLerp(s2::AnimLerp::SPR_POS, std::move(lerp));
	}
	else if (m_ease_pos->GetSelection() > 1 || m_ease_scale->GetSelection() > 1 || m_ease_rotate->GetSelection() > 1)
	{
		if (m_ease_pos->GetSelection() > 1) {
			auto lerp = std::make_unique<s2::LerpEase>(m_ease_pos->GetSelection());
			m_frame->SetLerp(s2::AnimLerp::SPR_POS, std::move(lerp));
		}
		if (m_ease_scale->GetSelection() > 1) {
			auto lerp = std::make_unique<s2::LerpEase>(m_ease_scale->GetSelection());
			m_frame->SetLerp(s2::AnimLerp::SPR_SCALE, std::move(lerp));
		}
		if (m_ease_rotate->GetSelection() > 1) {
			auto lerp = std::make_unique<s2::LerpEase>(m_ease_rotate->GetSelection());
			m_frame->SetLerp(s2::AnimLerp::SPR_ROTATE, std::move(lerp));
		}
	}
	else
	{
		m_frame->SetLerp(s2::AnimLerp::SPR_POS, NULL);
	}
}

void FrameLerpDialog::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(InitCircleLayout());
	sizer->AddSpacer(10);
	sizer->Add(InitSpiralLayout());
	sizer->AddSpacer(10);
	sizer->Add(InitWiggleLayout());	
	sizer->AddSpacer(10);
	sizer->Add(InitEaseLayout());	

	{
		SetEscapeId(wxID_CANCEL);
		sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxCENTER);
	}

	SetSizer(sizer);
	
	sizer->Layout();
	Refresh(true);
}

wxSizer* FrameLerpDialog::InitCircleLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	m_pos_circle = new wxCheckBox(this, wxID_ANY, ("位置circle插值"));
	top_sizer->Add(m_pos_circle);

	int circle_scale = 100;
	if (s2::ILerp* lerp = QueryLerp(m_frame->GetLerps(), s2::AnimLerp::SPR_POS, s2::LERP_CIRCLE))
	{
		m_pos_circle->SetValue(true);

		s2::LerpCircle* circle = static_cast<s2::LerpCircle*>(lerp);
		circle_scale = circle->GetScale() * 100;
	}

	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, "缩放"));

		m_circle_scale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, circle_scale);
		sizer->Add(m_circle_scale);

		top_sizer->Add(sizer);
	}


	return top_sizer;
}

wxSizer* FrameLerpDialog::InitSpiralLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	m_pos_spiral = new wxCheckBox(this, wxID_ANY, ("位置spiral插值"));
	top_sizer->Add(m_pos_spiral);

	int sprial_begin = 0, sprial_end = 0;
	int sprial_scale = 100;
	if (s2::ILerp* lerp = QueryLerp(m_frame->GetLerps(), s2::AnimLerp::SPR_POS, s2::LERP_SPIRAL))
	{
		m_pos_spiral->SetValue(true);

		s2::LerpSpiral* spiral = static_cast<s2::LerpSpiral*>(lerp);
		float begin, end;
		spiral->GetAngle(begin, end);
		sprial_begin = begin * SM_RAD_TO_DEG;
		sprial_end = end * SM_RAD_TO_DEG;
		sprial_scale = spiral->GetScale() * 100;
	}

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

	return top_sizer;
}

wxSizer* FrameLerpDialog::InitWiggleLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	m_pos_wiggle = new wxCheckBox(this, wxID_ANY, ("位置wiggle插值"));
	top_sizer->Add(m_pos_wiggle);

	float freq = 0;
	float amp = 0;
	if (s2::ILerp* lerp = QueryLerp(m_frame->GetLerps(), s2::AnimLerp::SPR_POS, s2::LERP_WIGGLE))
	{
		m_pos_wiggle->SetValue(true);

		s2::LerpWiggle* wiggle = static_cast<s2::LerpWiggle*>(lerp);
		freq = wiggle->GetFreq();
		amp = wiggle->GetAmp();
	}

	top_sizer->AddSpacer(10);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, "频率"));

		m_wiggle_freq = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
			wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, freq);
		sizer->Add(m_wiggle_freq);
	
		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(5);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, "幅度"));

		m_wiggle_amp = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
			wxDefaultSize, wxSP_ARROW_KEYS, -1000, 1000, amp);
		sizer->Add(m_wiggle_amp);

		top_sizer->Add(sizer);		
	}

	return top_sizer;
}

wxSizer* FrameLerpDialog::InitEaseLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	const int size = 32;
	wxString choices[size] = {
		"unknown",
		"linear",

		"in quad",
		"in cubic",
		"in quart",
		"in quint",
		"in sine",
		"in expo",
		"in circ",
		"in elastic",
		"in back",
		"in bounce",

		"out quad",
		"out cubic",
		"out quart",
		"out quint",
		"out sine",
		"out expo",
		"out circ",
		"out elastic",
		"out back",
		"out bounce",

		"in out quad",
		"in out cubic",
		"in out quart",
		"in out quint",
		"in out sine",
		"in out expo",
		"in out circ",
		"in out elastic",
		"in out back",
		"in out bounce"
	};

	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxTextCtrl(this, wxID_ANY, "位移"));

		m_ease_pos = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, size, choices);
		sizer->Add(m_ease_pos);

		if (s2::ILerp* lerp = QueryLerp(m_frame->GetLerps(), s2::AnimLerp::SPR_POS, s2::LERP_EASE)) {
			int type = static_cast<s2::LerpEase*>(lerp)->GetEaseType();
			m_ease_pos->SetSelection(type);
		} else {
			m_ease_pos->SetSelection(1);
		}

		top_sizer->Add(sizer);
	}
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxTextCtrl(this, wxID_ANY, "缩放"));

		m_ease_scale = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, size, choices);
		sizer->Add(m_ease_scale);

		if (s2::ILerp* lerp = QueryLerp(m_frame->GetLerps(), s2::AnimLerp::SPR_SCALE, s2::LERP_EASE)) {
			int type = static_cast<s2::LerpEase*>(lerp)->GetEaseType();
			m_ease_scale->SetSelection(type);
		} else {
			m_ease_scale->SetSelection(1);
		}

		top_sizer->Add(sizer);
	}
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxTextCtrl(this, wxID_ANY, "旋转"));

		m_ease_rotate = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, size, choices);
		sizer->Add(m_ease_rotate);

		if (s2::ILerp* lerp = QueryLerp(m_frame->GetLerps(), s2::AnimLerp::SPR_ROTATE, s2::LERP_EASE)) {
			int type = static_cast<s2::LerpEase*>(lerp)->GetEaseType();
			m_ease_rotate->SetSelection(type);
		} else {
			m_ease_rotate->SetSelection(1);
		}

		top_sizer->Add(sizer);
	}
	return top_sizer;
}

s2::ILerp* FrameLerpDialog::QueryLerp(const CU_VEC<std::pair<s2::AnimLerp::SprData, std::unique_ptr<s2::ILerp>>>& lerps,
									  int data_type, int lerp_type)
{
	auto itr = lerps.begin();
	for ( ; itr != lerps.end(); ++itr) {
		if (itr->first == data_type &&
			itr->second->Type() == lerp_type) {
			return itr->second.get();
		}
	}
	return nullptr;
}

}
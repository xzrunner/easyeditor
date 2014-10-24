#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

namespace eparticle2d
{

static const int COUNT = 20;

static const int EMISSION_TIME = 150;

static const float FADEOUT_TIME = 300;

static const int MIN_LIFE = 150, MAX_LIFE = 1300;

static const int POS_MIN = 0, POS_MAX = 0;

static const int SPD_MIN = 0, SPD_MAX = 0;

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library,
						   d2d::EditPanel* stage, ParticleSystem* ps)
	: d2d::ToolbarPanel(parent, stage)
	, m_ps(ps)
{
	m_ps = ps;
	m_ps->retain();

	SetScrollbars(1,1, 200, 100, 0, 0);
	SetSizer(initLayout());	

	SetDropTarget(new DropTarget(library, ps));

	InitPSValue();
}

ToolbarPanel::~ToolbarPanel()
{
	m_ps->release();
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	// Emission Time
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Emission Time(ms)"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_emission_time = new wxSlider(this, wxID_ANY, EMISSION_TIME, 10, 1000, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
		Connect(m_emission_time->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetEmissionTime));
		sizer->Add(m_emission_time);

		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(10);
	// Count
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Count"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_count = new wxSlider(this, wxID_ANY, COUNT, 1, 100, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
		Connect(m_count->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetCount));
		sizer->Add(m_count);

		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(10);
	// Fadeout Time
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Fadeout Time(ms)"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_fadeout_time = new wxSlider(this, wxID_ANY, FADEOUT_TIME, 10, 2500, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
		Connect(m_fadeout_time->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetFadeoutTime));
		sizer->Add(m_fadeout_time);

		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(10);
	// Life
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Life(ms)"));
		wxSizer* life_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min ")));

			m_life_min = new wxSlider(this, wxID_ANY, MIN_LIFE, 0, 5000, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_life_min->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetLife));
			sizer->Add(m_life_min);

			life_sizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max ")));

			m_life_max = new wxSlider(this, wxID_ANY, MAX_LIFE, 0, 5000, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_life_max->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetLife));
			sizer->Add(m_life_max);

			life_sizer->Add(sizer);
		}
		top_sizer->Add(life_sizer);
	}
	// Position
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Position"));
		wxSizer* pos_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min x")));

			m_pos_x_min = new wxSlider(this, wxID_ANY, POS_MIN, -200, 200, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_pos_x_min->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetPosition));
			sizer->Add(m_pos_x_min);

			pos_sizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max x")));

			m_pos_x_max = new wxSlider(this, wxID_ANY, POS_MAX, -200, 200, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_pos_x_max->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetPosition));
			sizer->Add(m_pos_x_max);

			pos_sizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min y")));

			m_pos_y_min = new wxSlider(this, wxID_ANY, POS_MIN, -200, 200, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_pos_y_min->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetPosition));
			sizer->Add(m_pos_y_min);

			pos_sizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max y")));

			m_pos_y_max = new wxSlider(this, wxID_ANY, POS_MAX, -200, 200, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_pos_y_max->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetPosition));
			sizer->Add(m_pos_y_max);

			pos_sizer->Add(sizer);
		}
		top_sizer->Add(pos_sizer);
	}
	top_sizer->AddSpacer(10);
	// Speed
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Speed"));
		wxSizer* spd_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min x")));

			m_spd_x_min = new wxSlider(this, wxID_ANY, SPD_MIN, -200, 200, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_spd_x_min->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetSpeed));
			sizer->Add(m_spd_x_min);

			spd_sizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max x")));

			m_spd_x_max = new wxSlider(this, wxID_ANY, SPD_MAX, -200, 200, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_spd_x_max->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetSpeed));
			sizer->Add(m_spd_x_max);

			spd_sizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min y")));

			m_spd_y_min = new wxSlider(this, wxID_ANY, SPD_MIN, -200, 200, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_spd_y_min->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetSpeed));
			sizer->Add(m_spd_y_min);

			spd_sizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max y")));

			m_spd_y_max = new wxSlider(this, wxID_ANY, SPD_MAX, -200, 200, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_spd_y_max->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::OnSetSpeed));
			sizer->Add(m_spd_y_max);

			spd_sizer->Add(sizer);
		}
		top_sizer->Add(spd_sizer);
	}
	top_sizer->AddSpacer(10);

	return top_sizer;
}

void ToolbarPanel::InitPSValue()
{
	if (!m_ps) {
		return;
	}

	OnSetCount(wxScrollEvent());
	OnSetEmissionTime(wxScrollEvent());
	OnSetFadeoutTime(wxScrollEvent());

	OnSetLife(wxScrollEvent());
	OnSetPosition(wxScrollEvent());
	OnSetSpeed(wxScrollEvent());
}

void ToolbarPanel::OnSetCount(wxScrollEvent& event)
{
	m_ps->SetCount(m_count->GetValue());
}

void ToolbarPanel::OnSetEmissionTime(wxScrollEvent& event)
{
	m_ps->SetEmissionTime(m_emission_time->GetValue());	
}

void ToolbarPanel::OnSetFadeoutTime(wxScrollEvent& event)
{
	m_ps->SetFadeoutTime(m_fadeout_time->GetValue());
}

void ToolbarPanel::OnSetLife(wxScrollEvent& event)
{
	m_ps->SetLife(m_life_min->GetValue(), m_life_max->GetValue());
}

void ToolbarPanel::OnSetPosition(wxScrollEvent& event)
{
	d2d::Vector min(m_pos_x_min->GetValue(), m_pos_y_min->GetValue());
	d2d::Vector max(m_pos_x_max->GetValue(), m_pos_y_max->GetValue());
	m_ps->SetPosition(min, max);
}

void ToolbarPanel::OnSetSpeed(wxScrollEvent& event)
{
	d2d::Vector min(m_spd_x_min->GetValue(), m_spd_y_min->GetValue());
	d2d::Vector max(m_spd_x_max->GetValue(), m_spd_y_max->GetValue());
	m_ps->SetSpeed(min, max);
}

//////////////////////////////////////////////////////////////////////////
// class ToolbarPanel::DropTarget
//////////////////////////////////////////////////////////////////////////

ToolbarPanel::DropTarget::
DropTarget(d2d::LibraryPanel* library, ParticleSystem* ps)
	: m_library(library)
	, m_ps(ps)
{
}

bool ToolbarPanel::DropTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	d2d::ISymbol* symbol = m_library->getSymbol(index);
	if (symbol)
	{
		m_ps->SetSymbol(symbol);
		m_ps->Start();
	}

	return true;
}

}
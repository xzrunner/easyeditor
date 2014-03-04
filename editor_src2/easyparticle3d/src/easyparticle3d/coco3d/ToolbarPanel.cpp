#include "ToolBarPanel.h"
#include "StagePanel.h"

#include "../Context.h"

using namespace eparticle::coco3d;

const float ToolbarPanel::COUNT				= 20;
const float ToolbarPanel::EMISSION_TIME		= 150;
const float ToolbarPanel::MIN_LIFE			= 300;
const float ToolbarPanel::MAX_LIFE			= 1300;
const float ToolbarPanel::MIN_HORI			= 0;
const float ToolbarPanel::MAX_HORI			= 360;
const float ToolbarPanel::MIN_VERT			= 60;
const float ToolbarPanel::MAX_VERT			= 90;
const float ToolbarPanel::MIN_SPD			= 1200;
const float ToolbarPanel::MAX_SPD			= 2000;
const float ToolbarPanel::GRAVITY			= 1200;
const float ToolbarPanel::FADEOUT_TIME		= 300;
const float ToolbarPanel::START_SCALE		= 50;
const float ToolbarPanel::END_SCALE			= 80;
const float ToolbarPanel::MIN_ROTATE		= -180;
const float ToolbarPanel::MAX_ROTATE		= 180;

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: d2d::ToolbarPanel(parent, Context::Instance()->stage)
	, m_image(NULL)
{
	SetSizer(initLayout());	
	enable(false);
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->AddSpacer(10);
	// Open
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Open..."));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::onOpenImage));
		topSizer->Add(btn);
	}
	topSizer->AddSpacer(10);
	// Count
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("count:")));

		m_count = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 1, 100, COUNT);
		Connect(m_count->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetCount));
		sizer->Add(m_count);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Emission Time
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("emission_time:")));

		m_emission_time = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 10, 1000, EMISSION_TIME);
		Connect(m_emission_time->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetEmissionTime));
		sizer->Add(m_emission_time);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Life
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Life:"));
		wxSizer* lifeSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min:")));	

			m_min_life = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 9999, MIN_LIFE);
			Connect(m_min_life->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetLife));
			sizer->Add(m_min_life);

			lifeSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max:")));	

			m_max_life = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 9999, MAX_LIFE);
			Connect(m_max_life->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetLife));
			sizer->Add(m_max_life);

			lifeSizer->Add(sizer);
		}
		topSizer->Add(lifeSizer);
	}
	topSizer->AddSpacer(10);
	// Hori
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Hori:"));
		wxSizer* horiSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min:")));	

			m_min_hori = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 360, MIN_HORI);
			Connect(m_min_hori->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetHori));
			sizer->Add(m_min_hori);

			horiSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max:")));	

			m_max_hori = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 360, MAX_HORI);
			Connect(m_max_hori->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetHori));
			sizer->Add(m_max_hori);

			horiSizer->Add(sizer);
		}
		topSizer->Add(horiSizer);
	}
	topSizer->AddSpacer(10);
	// Vert
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Vert:"));
		wxSizer* vertSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min:")));	

			m_min_vert = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 90, MIN_VERT);
			Connect(m_min_vert->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetVert));
			sizer->Add(m_min_vert);

			vertSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max:")));	

			m_max_vert = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 90, MAX_VERT);
			Connect(m_max_vert->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetVert));
			sizer->Add(m_max_vert);

			vertSizer->Add(sizer);
		}
		topSizer->Add(vertSizer);
	}
	topSizer->AddSpacer(10);
	// Speed
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Spd:"));
		wxSizer* spdSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min:")));	

			m_min_spd = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 9999, MIN_SPD);
			Connect(m_min_spd->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetSpeed));
			sizer->Add(m_min_spd);

			spdSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max:")));	

			m_max_spd = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 9999, MAX_SPD);
			Connect(m_max_spd->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetSpeed));
			sizer->Add(m_max_spd);

			spdSizer->Add(sizer);
		}
		topSizer->Add(spdSizer);
	}
	topSizer->AddSpacer(10);
	// Gravity
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("gravity:")));

		m_gravity = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 10, 50000, GRAVITY);
		Connect(m_gravity->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetGravity));
		sizer->Add(m_gravity);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Fadeout Time
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("fadeout_time:")));

		m_fadeout_time = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 10, 5000, GRAVITY);
		Connect(m_fadeout_time->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetFadeoutTime));
		sizer->Add(m_fadeout_time);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Bounce
	{
		m_bounce = new wxCheckBox(this, wxID_ANY, wxT("bounce"));
		Connect(m_bounce->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::onSetBounce));
		topSizer->Add(m_bounce);
	}
	topSizer->AddSpacer(10);
	// Scale
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("scale:"));
		wxSizer* scaleSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("start:")));	

			m_start_scale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 9999, START_SCALE);
			Connect(m_start_scale->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetScale));
			sizer->Add(m_start_scale);

			scaleSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("end:")));	

			m_end_scale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 9999, END_SCALE);
			Connect(m_end_scale->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetScale));
			sizer->Add(m_end_scale);

			scaleSizer->Add(sizer);
		}
		topSizer->Add(scaleSizer);
	}
	topSizer->AddSpacer(10);
	// Rotate
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("rotate:"));
		wxSizer* rotateSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min:")));	

			m_min_rotate = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, -180, 180, MIN_ROTATE);
			Connect(m_min_rotate->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetRotate));
			sizer->Add(m_min_rotate);

			rotateSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max:")));	

			m_max_rotate = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, -180, 180, MAX_ROTATE);
			Connect(m_max_rotate->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetRotate));
			sizer->Add(m_max_rotate);

			rotateSizer->Add(sizer);
		}
		topSizer->Add(rotateSizer);
	}
	return topSizer;
}

void ToolbarPanel::enable(bool enable)
{
	m_count->Enable(enable);
	m_emission_time->Enable(enable);
	m_min_life->Enable(enable);
	m_max_life->Enable(enable);
	m_min_hori->Enable(enable);
	m_max_hori->Enable(enable);
	m_min_vert->Enable(enable);
	m_max_vert->Enable(enable);
	m_min_spd->Enable(enable);
	m_max_spd->Enable(enable);
	m_gravity->Enable(enable);
	m_fadeout_time->Enable(enable);
	m_bounce->Enable(enable);
	m_start_scale->Enable(enable);
	m_end_scale->Enable(enable);
	m_min_rotate->Enable(enable);
	m_max_rotate->Enable(enable);
}

void ToolbarPanel::initParticle()
{
	coco::ParticleSystem* ps = Context::Instance()->stage->m_particle;

	ps->setCount(m_count->GetValue());
	ps->setEmissionTime(m_emission_time->GetValue());
	ps->setLife(m_min_life->GetValue(), m_max_life->GetValue());
	ps->setHori(m_min_hori->GetValue(), m_max_hori->GetValue());
	ps->setVert(m_min_vert->GetValue(), m_max_vert->GetValue());
	ps->setSpeed(m_min_spd->GetValue(), m_max_spd->GetValue());
	ps->setGravity(m_gravity->GetValue());
	ps->setFadeoutTime(m_fadeout_time->GetValue());
	ps->setScale(m_start_scale->GetValue(), m_end_scale->GetValue());
	ps->setRotate(m_min_rotate->GetValue(), m_max_rotate->GetValue());
}

void ToolbarPanel::onOpenImage(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Choose image"), wxEmptyString, 
		wxEmptyString, wxT("*.png;*.jpg"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		enable(true);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(dlg.GetPath());
		m_image = static_cast<d2d::ImageSymbol*>(symbol)->getImage();
		if (Context::Instance()->stage->m_particle)
			Context::Instance()->stage->m_particle->setImage(m_image);
		else
			Context::Instance()->stage->m_particle = new coco::ParticleSystem(m_image, 1000);
		initParticle();
	}
}

void ToolbarPanel::onSetCount(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setCount(m_count->GetValue());
}

void ToolbarPanel::onSetEmissionTime(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setEmissionTime(m_emission_time->GetValue());
}

void ToolbarPanel::onSetLife(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setLife(m_min_life->GetValue(), m_max_life->GetValue());
}

void ToolbarPanel::onSetHori(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setHori(m_min_hori->GetValue(), m_max_hori->GetValue());
}

void ToolbarPanel::onSetVert(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setVert(m_min_vert->GetValue(), m_max_vert->GetValue());
}

void ToolbarPanel::onSetSpeed(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setSpeed(m_min_spd->GetValue(), m_max_spd->GetValue());
}

void ToolbarPanel::onSetGravity(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setGravity(m_gravity->GetValue());
}

void ToolbarPanel::onSetFadeoutTime(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setFadeoutTime(m_fadeout_time->GetValue());
}

void ToolbarPanel::onSetBounce(wxCommandEvent& event)
{
	Context::Instance()->stage->m_particle->setBounce(event.IsChecked());
}

void ToolbarPanel::onSetScale(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setScale(m_start_scale->GetValue(), m_end_scale->GetValue());
}

void ToolbarPanel::onSetRotate(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setRotate(m_min_rotate->GetValue(), m_max_rotate->GetValue());
}
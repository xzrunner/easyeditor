#include "ToolBarPanel.h"
#include "StagePanel.h"

#include "../Context.h"

using namespace eparticle::coco;

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
const float ToolbarPanel::INERTIA			= 4;
const float ToolbarPanel::FADEOUT_TIME		= 300;
const float ToolbarPanel::START_RADIUS		= 0;
const float ToolbarPanel::START_SCALE		= 50;
const float ToolbarPanel::END_SCALE			= 80;
const float ToolbarPanel::MIN_ROTATE		= -180;
const float ToolbarPanel::MAX_ROTATE		= 180;

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: d2d::ToolbarPanel(parent, Context::Instance()->stage)
	, m_image(NULL)
{
	SetScrollbars(1,1, 200, 100, 0, 0);
	SetSizer(initLayout());	
	initParticle();
}

void ToolbarPanel::add(const d2d::ParticleFileAdapter::Child& child)
{
	ParticleChild* pc = new ParticleChild;
	ChildPanel* cp = new ChildPanel(this, pc);

	cp->m_filename->SetValue(child.filepath);
	cp->m_name->SetValue(child.name);
	cp->m_start_scale->SetValue(child.start_scale);
	cp->m_end_scale->SetValue(child.end_scale);
	cp->m_min_rotate->SetValue(child.min_rotate);
	cp->m_max_rotate->SetValue(child.max_rotate);
	cp->m_startz->SetValue(child.start_z);

	pc->symbol = d2d::SymbolMgr::Instance()->getSymbol(child.filepath);
	cp->onSetScale(wxSpinEvent());
	cp->onSetRotate(wxSpinEvent());

	m_compSizer->Insert(m_children.size(), cp);
	m_children.push_back(cp);
	Context::Instance()->stage->m_particle->addChild(pc);

	this->Layout();
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->AddSpacer(10);
	// Open
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		{
			wxButton* btn = new wxButton(this, wxID_ANY, wxT("Add"));
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::onAddChild));
			sizer->Add(btn);
		}
		sizer->AddSpacer(10);
		{
			wxButton* btn = new wxButton(this, wxID_ANY, wxT("Del"));
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::onDelChild));
			sizer->Add(btn);
		}
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Name
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("name:")));
		sizer->Add(m_name = new wxTextCtrl(this, wxID_ANY));
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Package
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("package:")));
		sizer->Add(m_package = new wxTextCtrl(this, wxID_ANY));
		topSizer->Add(sizer);
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
	// Layer
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("layer:")));
		sizer->Add(m_layer = new wxSpinCtrl(this));
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
		wxSizer* lifeSizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
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
		wxSizer* horiSizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
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
		wxSizer* vertSizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
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
		wxSizer* spdSizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
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
	// Inertia
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("inertia:")));

		m_inertia = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 1000, INERTIA);
		Connect(m_inertia->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetInertia));
		sizer->Add(m_inertia);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Fadeout Time
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("fadeout_time:")));

		m_fadeout_time = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 10, 5000, FADEOUT_TIME);
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
	// AdditiveBlend
	{
		m_additiveBlend = new wxCheckBox(this, wxID_ANY, wxT("additive blend"));
		Connect(m_additiveBlend->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::onSetAdditiveBlend));
		topSizer->Add(m_additiveBlend);
	}
	topSizer->AddSpacer(10);
	// Start Radius
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("start radius:")));

		m_start_radius = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 5000, START_RADIUS);
		Connect(m_start_radius->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetStartRadius));
		sizer->Add(m_start_radius);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// orient_to_movement
	{
		m_orient_to_movement = new wxCheckBox(this, wxID_ANY, wxT("orient to movement"));	
		topSizer->Add(m_orient_to_movement);
	}
	topSizer->AddSpacer(10);
	// orient_to_parent
	{
		m_orient_to_parent = new wxCheckBox(this, wxID_ANY, wxT("orient to parent"));
		topSizer->Add(m_orient_to_parent);
	}
	topSizer->AddSpacer(10);
 	// components
 	{
 		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("components"));
 		m_compSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		m_compSizer->AddSpacer(10);
 		topSizer->Add(m_compSizer);
 	}
	return topSizer;
}

void ToolbarPanel::initParticle()
{
	coco::ParticleSystem* ps = new coco::ParticleSystem(1000);
	ps->start();
	Context::Instance()->stage->m_particle = ps;

	ps->setCount(m_count->GetValue());
	ps->setEmissionTime(m_emission_time->GetValue());
	ps->setLife(m_min_life->GetValue(), m_max_life->GetValue());
	ps->setHori(m_min_hori->GetValue(), m_max_hori->GetValue());
	ps->setVert(m_min_vert->GetValue(), m_max_vert->GetValue());
	ps->setSpeed(m_min_spd->GetValue(), m_max_spd->GetValue());
	ps->setGravity(m_gravity->GetValue());
	ps->setInertia(m_inertia->GetValue());
	ps->setFadeoutTime(m_fadeout_time->GetValue());
}

void ToolbarPanel::onAddChild(wxCommandEvent& event)
{
	ParticleChild* pc = new ParticleChild;
	ChildPanel* cp = new ChildPanel(this, pc);
	m_compSizer->Insert(m_children.size(), cp);
	m_children.push_back(cp);
	Context::Instance()->stage->m_particle->addChild(pc);

// 	this->Fit();
//	m_parent->Fit();
	this->Layout();
//	m_parent->Layout();
//	m_parent->Refresh();
}

void ToolbarPanel::onDelChild(wxCommandEvent& event)
{
	if (m_children.empty()) return;

	m_compSizer->Detach(m_children.size()-1);
	delete m_children[m_children.size()-1];
	m_children.pop_back();
	Context::Instance()->stage->m_particle->delChild();

	this->Layout();
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

void ToolbarPanel::onSetInertia(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setInertia(m_inertia->GetValue());
}

void ToolbarPanel::onSetFadeoutTime(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setFadeoutTime(m_fadeout_time->GetValue());
}

void ToolbarPanel::onSetBounce(wxCommandEvent& event)
{
	Context::Instance()->stage->m_particle->setBounce(event.IsChecked());
}

void ToolbarPanel::onSetAdditiveBlend(wxCommandEvent& event)
{
	Context::Instance()->stage->m_particle->setAdditiveBlend(event.IsChecked());
}

void ToolbarPanel::onSetStartRadius(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setStartRadius(m_start_radius->GetValue());
}

//////////////////////////////////////////////////////////////////////////
// class ToolbarPanel::ChildPanel
//////////////////////////////////////////////////////////////////////////

ToolbarPanel::ChildPanel::
ChildPanel(wxWindow* parent, ParticleChild* pc)
	: wxPanel(parent)
	, m_pc(pc)
{
	initLayout();

	m_pc->start_scale = m_start_scale->GetValue() * 0.01f;
	m_pc->end_scale = m_end_scale->GetValue() * 0.01f;
	m_pc->min_rotate = m_min_rotate->GetValue() * d2d::TRANS_DEG_TO_RAD * 60;
	m_pc->max_rotate = m_max_rotate->GetValue() * d2d::TRANS_DEG_TO_RAD * 60;
}

void ToolbarPanel::ChildPanel::
initLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->AddSpacer(10);
	// Open
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Open..."));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ChildPanel::onSetImage));
		sizer->Add(btn);

		m_filename = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
		sizer->Add(m_filename);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Name
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("name:")));
		sizer->Add(m_name = new wxTextCtrl(this, wxID_ANY));
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Scale
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("scale:"));
		wxSizer* scaleSizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("start:")));	

			m_start_scale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 9999, START_SCALE);
			Connect(m_start_scale->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ChildPanel::onSetScale));
			sizer->Add(m_start_scale);

			scaleSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("end:")));	

			m_end_scale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 9999, END_SCALE);
			Connect(m_end_scale->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ChildPanel::onSetScale));
			sizer->Add(m_end_scale);

			scaleSizer->Add(sizer);
		}
		topSizer->Add(scaleSizer);
	}
	topSizer->AddSpacer(10);
	// Rotate
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("rotate:"));
		wxSizer* rotateSizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min:")));	

			m_min_rotate = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, -180, 180, MIN_ROTATE);
			Connect(m_min_rotate->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ChildPanel::onSetRotate));
			sizer->Add(m_min_rotate);

			rotateSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max:")));	

			m_max_rotate = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, -180, 180, MAX_ROTATE);
			Connect(m_max_rotate->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ChildPanel::onSetRotate));
			sizer->Add(m_max_rotate);

			rotateSizer->Add(sizer);
		}
		topSizer->Add(rotateSizer);
	}
	topSizer->AddSpacer(10);
	// Name
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("startz:")));
		sizer->Add(m_startz = new wxSpinCtrl(this));
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	//
	SetSizer(topSizer);
}

void ToolbarPanel::ChildPanel::
onSetImage(wxCommandEvent& event)
{
	wxString filter = wxT("*.png;*.jpg");
	filter += wxT(";*_complex.json");
	filter += wxT(";*_anim.json");
	wxFileDialog dlg(this, wxT("Choose Symbol"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_pc->symbol = d2d::SymbolMgr::Instance()->getSymbol(dlg.GetPath());
		m_filename->SetLabelText(m_pc->symbol->getFilepath());
		Context::Instance()->stage->m_particle->start();
	}
}

void ToolbarPanel::ChildPanel::
onSetScale(wxSpinEvent& event)
{
	m_pc->start_scale = m_start_scale->GetValue() * 0.01f;
	m_pc->end_scale = m_end_scale->GetValue() * 0.01f;
}

void ToolbarPanel::ChildPanel::
onSetRotate(wxSpinEvent& event)
{
	m_pc->min_rotate = m_min_rotate->GetValue() * d2d::TRANS_DEG_TO_RAD;
	m_pc->max_rotate = m_max_rotate->GetValue() * d2d::TRANS_DEG_TO_RAD;
}

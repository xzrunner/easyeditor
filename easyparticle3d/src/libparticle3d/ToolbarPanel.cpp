#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "config.h"

#include <easyanim.h>

namespace eparticle3d
{

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
const float ToolbarPanel::MIN_ANGULAR_SPE	= 0;
const float ToolbarPanel::MAX_ANGULAR_SPE	= 0;
const float ToolbarPanel::GRAVITY			= 1200;
const float ToolbarPanel::INERTIA			= 4;
const float ToolbarPanel::FADEOUT_TIME		= 300;
const float ToolbarPanel::START_RADIUS		= 0;
const float ToolbarPanel::START_SCALE		= 50;
const float ToolbarPanel::END_SCALE			= 80;
const float ToolbarPanel::MIN_ROTATE		= -180;
const float ToolbarPanel::MAX_ROTATE		= 180;

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library,
						   StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
	, m_stage(stage)
	, m_image(NULL)
{
	SetScrollbars(1,1, 200, 100, 0, 0);
	SetSizer(initLayout());	
	initParticle();

	SetDropTarget(new DropTarget(library, stage, this));
}

void ToolbarPanel::add(const FileAdapter::Child& child)
{
	ParticleChild* pc = new ParticleChild;
	ChildPanel* cp = new ChildPanel(this, pc);

	cp->m_name->SetValue(child.name);
	cp->m_start_scale->SetValue(child.start_scale);
	cp->m_end_scale->SetValue(child.end_scale);
	cp->m_min_rotate->SetValue(child.min_rotate);
	cp->m_max_rotate->SetValue(child.max_rotate);
	cp->m_startz->SetValue(child.start_z);

	// todo Release symbol
	pc->symbol = d2d::SymbolMgr::Instance()->fetchSymbol(child.filepath);
	if (!child.bind_filepath.empty()) {
		pc->bind_ps = FileIO::LoadPS(child.bind_filepath.c_str());
	}
	cp->onSetScale(wxScrollEvent());
	cp->onSetRotate(wxScrollEvent());

	m_compSizer->Insert(m_children.size(), cp);
	m_children.push_back(cp);
	m_stage->m_ps->addChild(pc);

	this->Layout();
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->AddSpacer(10);

	wxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
	wxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(leftSizer);
	topSizer->AddSpacer(10);
	topSizer->Add(rightSizer);

	leftSizer->AddSpacer(10);
	// Record
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Store Record"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::onStoreRecord));
		leftSizer->Add(btn);
	}
	leftSizer->AddSpacer(20);
	// Name
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Name ")));
		sizer->Add(m_name = new wxTextCtrl(this, wxID_ANY));
		leftSizer->Add(sizer);
	}
	leftSizer->AddSpacer(10);
	// Package
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Package ")));
		sizer->Add(m_package = new wxTextCtrl(this, wxID_ANY));
		leftSizer->Add(sizer);
	}
	leftSizer->AddSpacer(10);
	// Count
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Count"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_count = new wxSlider(this, wxID_ANY, COUNT, 1, 100, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
		Connect(m_count->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::onSetCount));
		sizer->Add(m_count);

		leftSizer->Add(sizer);
	}
	leftSizer->AddSpacer(10);
	// Layer
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Layer ")));
		sizer->Add(m_layer = new wxSpinCtrl(this));
		leftSizer->Add(sizer);
	}
	leftSizer->AddSpacer(10);
	// Emission Time
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Emission Time (ms)"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_emission_time = new wxSlider(this, wxID_ANY, EMISSION_TIME, 10, 1000, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
		Connect(m_emission_time->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::onSetEmissionTime));
		sizer->Add(m_emission_time);

		leftSizer->Add(sizer);
	}
	leftSizer->AddSpacer(10);
	// Life
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Life (ms)"));
		wxSizer* lifeSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min ")));

			m_min_life = new wxSlider(this, wxID_ANY, MIN_LIFE, 0, 5000, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_min_life->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::onSetLife));
			sizer->Add(m_min_life);

			lifeSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max ")));

			m_max_life = new wxSlider(this, wxID_ANY, MAX_LIFE, 0, 5000, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_max_life->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::onSetLife));
			sizer->Add(m_max_life);

			lifeSizer->Add(sizer);
		}
		leftSizer->Add(lifeSizer);
	}
	leftSizer->AddSpacer(10);
	// Hori
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Horizontal (deg)"));
		wxSizer* horiSizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min ")));	

			m_min_hori = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 360, MIN_HORI);
			Connect(m_min_hori->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetHori));
			sizer->Add(m_min_hori);

			horiSizer->Add(sizer);
		}
		horiSizer->AddSpacer(10);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max ")));	

			m_max_hori = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 360, MAX_HORI);
			Connect(m_max_hori->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetHori));
			sizer->Add(m_max_hori);

			horiSizer->Add(sizer);
		}
		leftSizer->Add(horiSizer);
	}
	leftSizer->AddSpacer(10);
	// Vert
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Vertical (deg)"));
		wxSizer* vertSizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min ")));

			m_min_vert = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 90, MIN_VERT);
			Connect(m_min_vert->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetVert));
			sizer->Add(m_min_vert);

			vertSizer->Add(sizer);
		}
		vertSizer->AddSpacer(10);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max ")));	

			m_max_vert = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 90, MAX_VERT);
			Connect(m_max_vert->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetVert));
			sizer->Add(m_max_vert);

			vertSizer->Add(sizer);
		}
		leftSizer->Add(vertSizer);
	}
	leftSizer->AddSpacer(10);
	// Speed
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Speed (pixel)"));
		wxSizer* spdSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min ")));

			m_min_spd = new wxSlider(this, wxID_ANY, MIN_SPD, 0, 9000, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_min_spd->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::onSetSpeed));
			sizer->Add(m_min_spd);

			spdSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max ")));

			m_max_spd = new wxSlider(this, wxID_ANY, MAX_SPD, 0, 9000, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_max_spd->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::onSetSpeed));
			sizer->Add(m_max_spd);

			spdSizer->Add(sizer);
		}
		leftSizer->Add(spdSizer);
	}
	leftSizer->AddSpacer(10);
	// Angular Speed
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Angular Speed (degree)"));
		wxSizer* spdSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min ")));

			m_min_angular_spd = new wxSlider(this, wxID_ANY, MIN_ANGULAR_SPE, -3600, 3600, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_min_angular_spd->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::onSetAngularSpeed));
			sizer->Add(m_min_angular_spd);

			spdSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max ")));

			m_max_angular_spd = new wxSlider(this, wxID_ANY, MAX_ANGULAR_SPE, -3600, 3600, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_max_angular_spd->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::onSetAngularSpeed));
			sizer->Add(m_max_angular_spd);

			spdSizer->Add(sizer);
		}
		leftSizer->Add(spdSizer);
	}
	// Gravity
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Gravity (pixel)"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_gravity = new wxSlider(this, wxID_ANY, GRAVITY, -1000, 9999, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
		Connect(m_gravity->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::onSetGravity));
		sizer->Add(m_gravity);

		leftSizer->Add(sizer);
	}
	leftSizer->AddSpacer(10);
	// Inertia
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Inertia ")));

		m_inertia = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 1000, INERTIA);
		Connect(m_inertia->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetInertia));
		sizer->Add(m_inertia);

		leftSizer->Add(sizer);
	}
	leftSizer->AddSpacer(10);
	// Fadeout Time
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Fadeout Time (ms)"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_fadeout_time = new wxSlider(this, wxID_ANY, FADEOUT_TIME, 10, 2500, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
		Connect(m_fadeout_time->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::onSetFadeoutTime));
		sizer->Add(m_fadeout_time);

		leftSizer->Add(sizer);
	}
	leftSizer->AddSpacer(10);
	// Bounce
	{
		m_bounce = new wxCheckBox(this, wxID_ANY, wxT("Bounce"));
		Connect(m_bounce->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::onSetBounce));
		leftSizer->Add(m_bounce);
	}
	leftSizer->AddSpacer(10);
	// AdditiveBlend
	{
		m_additiveBlend = new wxCheckBox(this, wxID_ANY, wxT("Additive Blend"));
		Connect(m_additiveBlend->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::onSetAdditiveBlend));
		leftSizer->Add(m_additiveBlend);
	}
	leftSizer->AddSpacer(10);
	// Start Radius
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Start Radius (pixel)"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_start_radius = new wxSlider(this, wxID_ANY, START_RADIUS, 0, 1000, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
		Connect(m_start_radius->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ToolbarPanel::onSetStartRadius));
		sizer->Add(m_start_radius);

		leftSizer->Add(sizer);
	}
	leftSizer->AddSpacer(10);
	// orient_to_movement
	{
		m_orient_to_movement = new wxCheckBox(this, wxID_ANY, wxT("Orient to Movement"));	
		Connect(m_orient_to_movement->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetOrientToMovement));
		leftSizer->Add(m_orient_to_movement);
	}
	leftSizer->AddSpacer(10);
	// orient_to_parent
	{
		m_orient_to_parent = new wxCheckBox(this, wxID_ANY, wxT("Orient to Parent"));
		leftSizer->Add(m_orient_to_parent);
	}
	leftSizer->AddSpacer(10);

	rightSizer->AddSpacer(10);
 	// components
 	{
		// Open
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Remove"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::onDelChild));
		rightSizer->Add(btn);
		rightSizer->AddSpacer(10);

 		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Components"));
 		m_compSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		m_compSizer->AddSpacer(10);
 		rightSizer->Add(m_compSizer);
 	}
	return topSizer;
}

void ToolbarPanel::initParticle()
{
	clear();

	ParticleSystem* ps = new ParticleSystem(PARTICLE_CAP);
	ps->start();
	m_stage->m_ps = ps;

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

void ToolbarPanel::clear()
{
	while (!m_children.empty())
	{
		m_compSizer->Detach(m_children.size()-1);
		delete m_children[m_children.size()-1];
		m_children.pop_back();
		m_stage->m_ps->delChild();
	}
	this->Layout();
}

void ToolbarPanel::onStoreRecord(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString,
		wxT("*_") + wxString(libanim::FILE_TAG) + wxT(".json"), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString filepath = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), libanim::FILE_TAG, "json");
		m_stage->m_ps->StoreRecordAsAnimFile(filepath.ToStdString());
	}
}

void ToolbarPanel::onAddChild(wxCommandEvent& event)
{
	ParticleChild* pc = new ParticleChild;
	ChildPanel* cp = new ChildPanel(this, pc);
	m_compSizer->Insert(m_children.size(), cp);
	m_children.push_back(cp);
	m_stage->m_ps->addChild(pc);

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
	m_stage->m_ps->delChild();

	this->Layout();
}

void ToolbarPanel::onSetCount(wxScrollEvent& event)
{
	m_stage->m_ps->setCount(m_count->GetValue());
}

void ToolbarPanel::onSetEmissionTime(wxScrollEvent& event)
{
	m_stage->m_ps->setEmissionTime(m_emission_time->GetValue());
}

void ToolbarPanel::onSetLife(wxScrollEvent& event)
{
	m_stage->m_ps->setLife(m_min_life->GetValue(), m_max_life->GetValue());
}

void ToolbarPanel::onSetHori(wxSpinEvent& event)
{
	m_stage->m_ps->setHori(m_min_hori->GetValue(), m_max_hori->GetValue());
}

void ToolbarPanel::onSetVert(wxSpinEvent& event)
{
	m_stage->m_ps->setVert(m_min_vert->GetValue(), m_max_vert->GetValue());
}

void ToolbarPanel::onSetSpeed(wxScrollEvent& event)
{
	m_stage->m_ps->setSpeed(m_min_spd->GetValue(), m_max_spd->GetValue());
}

void ToolbarPanel::onSetAngularSpeed(wxScrollEvent& event)
{
	m_stage->m_ps->setAngularSpeed(m_min_angular_spd->GetValue(), m_max_angular_spd->GetValue());
}

void ToolbarPanel::onSetGravity(wxScrollEvent& event)
{
	m_stage->m_ps->setGravity(m_gravity->GetValue());
}

void ToolbarPanel::onSetInertia(wxSpinEvent& event)
{
	m_stage->m_ps->setInertia(m_inertia->GetValue());
}

void ToolbarPanel::onSetFadeoutTime(wxScrollEvent& event)
{
	m_stage->m_ps->setFadeoutTime(m_fadeout_time->GetValue());
}

void ToolbarPanel::onSetBounce(wxCommandEvent& event)
{
	m_stage->m_ps->setBounce(event.IsChecked());
}

void ToolbarPanel::onSetAdditiveBlend(wxCommandEvent& event)
{
	m_stage->m_ps->setAdditiveBlend(event.IsChecked());
}

void ToolbarPanel::onSetStartRadius(wxScrollEvent& event)
{
	m_stage->m_ps->setStartRadius(m_start_radius->GetValue());
}

void ToolbarPanel::OnSetOrientToMovement(wxCommandEvent& event)
{
	m_stage->m_ps->SetOrientToMovement(event.IsChecked());
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
	// Name
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Name")));
		sizer->Add(m_name = new wxTextCtrl(this, wxID_ANY));
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Scale
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Scale (%)"));
		wxSizer* scaleSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("start ")));

			m_start_scale = new wxSlider(this, wxID_ANY, START_SCALE, 0, 500, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_start_scale->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ChildPanel::onSetScale));
			sizer->Add(m_start_scale);

			scaleSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("end ")));

			m_end_scale = new wxSlider(this, wxID_ANY, END_SCALE, 0, 500, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_end_scale->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ChildPanel::onSetScale));
			sizer->Add(m_end_scale);

			scaleSizer->Add(sizer);
		}
		topSizer->Add(scaleSizer);
	}
	topSizer->AddSpacer(10);
	// Rotate
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Rotate (deg)"));
		wxSizer* rotateSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min ")));

			m_min_rotate = new wxSlider(this, wxID_ANY, MIN_ROTATE, -180, 180, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_min_rotate->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ChildPanel::onSetRotate));
			sizer->Add(m_min_rotate);

			rotateSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max ")));

			m_max_rotate = new wxSlider(this, wxID_ANY, MAX_ROTATE, -180, 180, wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
			Connect(m_max_rotate->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(ChildPanel::onSetRotate));
			sizer->Add(m_max_rotate);

			rotateSizer->Add(sizer);
		}
		topSizer->Add(rotateSizer);
	}
	topSizer->AddSpacer(10);
	// Name
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Startz ")));
		sizer->Add(m_startz = new wxSpinCtrl(this));
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Bind PS
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Bind PS"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::ChildPanel::OnBindPS));
		topSizer->Add(btn);
	}
	//
	SetSizer(topSizer);
}

void ToolbarPanel::ChildPanel::
onSetScale(wxScrollEvent& event)
{
	m_pc->start_scale = m_start_scale->GetValue() * 0.01f;
	m_pc->end_scale = m_end_scale->GetValue() * 0.01f;
}

void ToolbarPanel::ChildPanel::
onSetRotate(wxScrollEvent& event)
{
	m_pc->min_rotate = m_min_rotate->GetValue() * d2d::TRANS_DEG_TO_RAD;
	m_pc->max_rotate = m_max_rotate->GetValue() * d2d::TRANS_DEG_TO_RAD;
}

void ToolbarPanel::ChildPanel::
OnBindPS(wxCommandEvent& event)
{
	wxString filter = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_particle3d);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入Particle3D文件"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_pc->bind_ps = FileIO::LoadPS(dlg.GetPath());

// 		if (m_canvas) {
// 			m_canvas->resetViewport();
// 		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class ToolbarPanel::DropTarget
//////////////////////////////////////////////////////////////////////////

ToolbarPanel::DropTarget::
DropTarget(d2d::LibraryPanel* library, StagePanel* stage,
		   ToolbarPanel* toolbar)
	: m_library(library)
	, m_stage(stage)
	, m_toolbar(toolbar)
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
		m_toolbar->onAddChild(wxCommandEvent());

		ToolbarPanel::ChildPanel* child = m_toolbar->m_children.back();
		child->m_pc->symbol = m_library->getSymbol(index);
		m_stage->m_ps->start();
		m_stage->ResetViewport();
	}

	return true;
}


}
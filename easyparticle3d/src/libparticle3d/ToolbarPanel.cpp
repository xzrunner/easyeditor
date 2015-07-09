#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "config.h"
#include "ps_config.h"
#include "item_string.h"

#include <easyanim.h>

namespace eparticle3d
{

static const float COUNT				= 20;
static const float EMISSION_TIME		= 150;
static const float LIFE_CENTER			= 800;
static const float LIFE_OFFSET			= 500;
static const float MIN_HORI				= 0;
static const float MAX_HORI				= 360;
static const float MIN_VERT				= 60;
static const float MAX_VERT				= 90;
static const float SPEED_CENTER			= 1600;
static const float SPEED_OFFSET			= 400;
static const float ANGULAR_SPEED_CENTER = 0;
static const float ANGULAR_SPEED_OFFSET = 0;
static const float DISTURBANCE_RADIUS_CENTER = 0;
static const float DISTURBANCE_RADIUS_OFFSET = 0;
static const float DISTURBANCE_SPD_CENTER = 0;
static const float DISTURBANCE_SPD_OFFSET = 0;
static const float GRAVITY				= 1200;
static const float LINEAR_ACC_CENTER	= 0;
static const float LINEAR_ACC_OFFSET	= 0;
static const float INERTIA				= 4;
static const float FADEOUT_TIME			= 300;
static const float START_RADIUS			= 0;
static const float SCALE_START			= 100;
static const float SCALE_END			= 100;
static const float ROTATE_MIN			= 0;
static const float ROTATE_MAX			= 0;

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

void ToolbarPanel::SetValue(int key, const d2d::UICallback::Data& data)
{
	if (m_stage->m_ps) {
		m_stage->m_ps->SetValue(key, data);
	}
}

void ToolbarPanel::GetValue(int key, d2d::UICallback::Data& data)
{
	if (m_stage->m_ps) {
		m_stage->m_ps->GetValue(key, data);
	}	
}

void ToolbarPanel::Load(const Json::Value& val, int version)
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Load(val, version);
		m_sliders[i]->Update();
	}
}

void ToolbarPanel::Store(Json::Value& val) const
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Store(val);
	}
}

void ToolbarPanel::add(const FileAdapter::Child& child)
{
	ParticleChild* pc = new ParticleChild;
	ChildPanel* cp = new ChildPanel(this, pc);

	cp->m_name->SetValue(child.name);
	cp->SetValue(PS_SCALE, d2d::UICallback::Data(child.start_scale, child.end_scale));
	cp->SetValue(PS_ROTATE, d2d::UICallback::Data(child.min_rotate, child.max_rotate));
	for (int i = 0, n = cp->m_sliders.size(); i < n; ++i) {
		cp->m_sliders[i]->Load();
	}
	cp->m_startz->SetValue(child.start_z);

	// todo Release symbol
	pc->symbol = d2d::SymbolMgr::Instance()->FetchSymbol(child.filepath);
	if (!child.bind_filepath.empty()) {
		pc->bind_ps = FileIO::LoadPS(child.bind_filepath.c_str());
	}

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
	d2d::SliderCtrlOne* s_count = new d2d::SliderCtrlOne(this, "Count", "count", 
		this, PS_COUNT, d2d::SliderItem("", "", COUNT, 1, 100));
	leftSizer->Add(s_count);
	leftSizer->AddSpacer(10);
	m_sliders.push_back(s_count);
	// Layer
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Layer ")));
		sizer->Add(m_layer = new wxSpinCtrl(this));
		leftSizer->Add(sizer);
	}
	leftSizer->AddSpacer(10);
	// Emission Time
	d2d::SliderCtrlOne* s_emission_time = new d2d::SliderCtrlOne(this, "Emission Time (ms)", "emission_time",
		this, PS_EMISSION_TIME, d2d::SliderItem("", "", EMISSION_TIME, 10, 1000));
	leftSizer->Add(s_emission_time);
	leftSizer->AddSpacer(10);
	m_sliders.push_back(s_emission_time);
	// Life
	d2d::SliderCtrlTwo* s_life = new d2d::SliderCtrlTwo(this, "Life (ms)", "life", this, PS_LIFE_TIME, 
		d2d::SliderItem("center", ITEM_ATTR_CENTER, LIFE_CENTER, 0, 5000), d2d::SliderItem("offset", ITEM_ATTR_OFFSET, LIFE_OFFSET, 0, 2500));
	leftSizer->Add(s_life);
	leftSizer->AddSpacer(10);
	m_sliders.push_back(s_life);
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
	d2d::SliderCtrlTwo* s_spd = new d2d::SliderCtrlTwo(this, "Speed (pixel)", "speed", this, PS_SPEED, 
		d2d::SliderItem("center", ITEM_ATTR_CENTER, SPEED_CENTER, 0, 9000), d2d::SliderItem("offset", ITEM_ATTR_OFFSET, SPEED_OFFSET, 0, 4000));
	leftSizer->Add(s_spd);
	leftSizer->AddSpacer(10);
	m_sliders.push_back(s_spd);
	// Angular Speed
	d2d::SliderCtrlTwo* s_aspd = new d2d::SliderCtrlTwo(this, "Angular Speed (degree)", "angular_speed", this, PS_ANGULAR_SPEED, 
		d2d::SliderItem("center", ITEM_ATTR_CENTER, ANGULAR_SPEED_CENTER, -3600, 3600), d2d::SliderItem("offset", ITEM_ATTR_OFFSET, ANGULAR_SPEED_OFFSET, 0, 360));
	leftSizer->Add(s_aspd);
	leftSizer->AddSpacer(10);
	m_sliders.push_back(s_aspd);
	// Disturbance 
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Disturbance"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		d2d::SliderCtrlTwo* s_dis_r = new d2d::SliderCtrlTwo(this, "Radius (pixel)", "disturbance_radius", this, PS_DISTURBANCE_RADIUS, 
			d2d::SliderItem("center", ITEM_ATTR_CENTER, DISTURBANCE_RADIUS_CENTER, 0, 100), d2d::SliderItem("offset", ITEM_ATTR_OFFSET, DISTURBANCE_RADIUS_OFFSET, 0, 50));
		sizer->Add(s_dis_r);
		m_sliders.push_back(s_dis_r);

		d2d::SliderCtrlTwo* s_dis_spd = new d2d::SliderCtrlTwo(this, "Speed (pixel)", "disturbance_spd", this, PS_DISTURBANCE_SPD, 
			d2d::SliderItem("center", ITEM_ATTR_CENTER, DISTURBANCE_SPD_CENTER, 0, 100), d2d::SliderItem("offset", ITEM_ATTR_OFFSET, DISTURBANCE_SPD_OFFSET, 0, 50));
		sizer->Add(s_dis_spd);
		m_sliders.push_back(s_dis_spd);

		leftSizer->Add(sizer);
		leftSizer->AddSpacer(10);
	}
	// Gravity
	d2d::SliderCtrlOne* s_gravity = new d2d::SliderCtrlOne(this, "Gravity (pixel)", "gravity", 
		this, PS_GRAVITY, d2d::SliderItem("", "", GRAVITY, -5000, 25000));
	leftSizer->Add(s_gravity);
	leftSizer->AddSpacer(10);
	m_sliders.push_back(s_gravity);
	// Linear Acc
	d2d::SliderCtrlTwo* s_lacc = new d2d::SliderCtrlTwo(this, "Linear Acc (pixel)", "linear_acc", this, PS_LINEAR_ACC, 
		d2d::SliderItem("center", ITEM_ATTR_CENTER, LINEAR_ACC_CENTER, -200, 500), d2d::SliderItem("offset", ITEM_ATTR_OFFSET, LINEAR_ACC_OFFSET, 0, 500));
	leftSizer->Add(s_lacc);
	leftSizer->AddSpacer(10);
	m_sliders.push_back(s_lacc);	
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
	d2d::SliderCtrlOne* s_fadeout = new d2d::SliderCtrlOne(this, "Fadeout Time (ms)", "fadeout_time", 
		this, PS_FADEOUT_TIME, d2d::SliderItem("", "", FADEOUT_TIME, 10, 2500));
	leftSizer->Add(s_fadeout);
	leftSizer->AddSpacer(10);
	m_sliders.push_back(s_fadeout);
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
	d2d::SliderCtrlOne* s_start_radius = new d2d::SliderCtrlOne(this, "Start Radius (pixel)", "start_radius", 
		this, PS_START_RADIUS, d2d::SliderItem("", "", START_RADIUS, 0, 1000));
	leftSizer->Add(s_start_radius);
	leftSizer->AddSpacer(10);
	m_sliders.push_back(s_start_radius);
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

	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}

	ps->setHori(m_min_hori->GetValue(), m_max_hori->GetValue());
	ps->setVert(m_min_vert->GetValue(), m_max_vert->GetValue());
	ps->setInertia(m_inertia->GetValue());
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

void ToolbarPanel::onSetHori(wxSpinEvent& event)
{
	m_stage->m_ps->setHori(m_min_hori->GetValue(), m_max_hori->GetValue());
}

void ToolbarPanel::onSetVert(wxSpinEvent& event)
{
	m_stage->m_ps->setVert(m_min_vert->GetValue(), m_max_vert->GetValue());
}

void ToolbarPanel::onSetInertia(wxSpinEvent& event)
{
	m_stage->m_ps->setInertia(m_inertia->GetValue());
}

void ToolbarPanel::onSetBounce(wxCommandEvent& event)
{
	m_stage->m_ps->setBounce(event.IsChecked());
}

void ToolbarPanel::onSetAdditiveBlend(wxCommandEvent& event)
{
	m_stage->m_ps->setAdditiveBlend(event.IsChecked());
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
	InitLayout();
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}
}

void ToolbarPanel::ChildPanel::
SetValue(int key, const d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_SCALE:
		m_pc->start_scale = data.val0 * 0.01f;
		m_pc->end_scale = data.val1 * 0.01f;
		break;
	case PS_ROTATE:
		m_pc->min_rotate = data.val0 * d2d::TRANS_DEG_TO_RAD;
		m_pc->max_rotate = data.val1 * d2d::TRANS_DEG_TO_RAD;
		break;
	}
}

void ToolbarPanel::ChildPanel::
GetValue(int key, d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_SCALE:
		data.val0 = m_pc->start_scale * 100;
		data.val1 = m_pc->end_scale * 100;
		break;
	case PS_ROTATE:
		data.val0 = m_pc->min_rotate * d2d::TRANS_RAD_TO_DEG * 0.5f;
		data.val1 = m_pc->max_rotate * d2d::TRANS_RAD_TO_DEG * 0.5f;
		break;
	}
}

void ToolbarPanel::ChildPanel::
InitLayout()
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
	d2d::SliderCtrlTwo* s_scale = new d2d::SliderCtrlTwo(this, "Scale (%)", "scale", this, PS_SCALE, 
		d2d::SliderItem("start", "start", SCALE_START, 0, 500), d2d::SliderItem("end", "end", SCALE_END, 0, 500));
	topSizer->Add(s_scale);
	topSizer->AddSpacer(10);
	m_sliders.push_back(s_scale);
	// Rotate
	d2d::SliderCtrlTwo* s_rotate = new d2d::SliderCtrlTwo(this, "Rotate (deg)", "rotate", this, PS_ROTATE, 
		d2d::SliderItem("min", "min", ROTATE_MIN, -180, 180), d2d::SliderItem("max", "max", ROTATE_MAX, -180, 180));
	topSizer->Add(s_rotate);
	topSizer->AddSpacer(10);
	m_sliders.push_back(s_rotate);
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
OnBindPS(wxCommandEvent& event)
{
	wxString filter = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_particle3d);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入Particle3D文件"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_pc->bind_ps = FileIO::LoadPS(dlg.GetPath());

// 		if (m_canvas) {
// 			m_canvas->ResetViewport();
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

	d2d::ISymbol* symbol = m_library->GetSymbol(index);
	if (symbol)
	{
		m_toolbar->onAddChild(wxCommandEvent());

		ToolbarPanel::ChildPanel* child = m_toolbar->m_children.back();
		child->m_pc->symbol = m_library->GetSymbol(index);
		m_stage->m_ps->start();
		m_stage->ResetViewport();
	}

	return true;
}


}
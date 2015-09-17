#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "config.h"
#include "ps_config.h"
#include "item_string.h"
#include "PSConfigMgr.h"
#include "ComponentPanel.h"
#include "language.h"

#include <particle3d.h>

#include <easyanim.h>

namespace eparticle3d
{

static const float CAPACITY				= 100;
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

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library,
						   StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage->GetStageImpl())
	, m_stage(stage)
	, m_image(NULL)
{
	SetScrollbars(1,1, 200, 100, 0, 0);
	SetSizer(initLayout());	
	InitParticle();

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

void ToolbarPanel::Add(const FileAdapter::Component& comp)
{
	// todo Release symbol
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(comp.filepath);
	p3d_symbol* pc = m_stage->m_ps->AddSymbol(symbol);
	ComponentPanel* cp = new ComponentPanel(this, pc, this);

	cp->m_name->SetValue(comp.name);

	cp->SetValue(PS_SCALE, d2d::UICallback::Data(comp.scale_start, comp.scale_end));

	cp->SetValue(PS_ROTATE, d2d::UICallback::Data(comp.angle - comp.angle_var, comp.angle + comp.angle_var));

	memcpy(&pc->col_mul.r, &comp.col_mul.r, sizeof(pc->col_mul));
	memcpy(&pc->col_add.r, &comp.col_add.r, sizeof(pc->col_add));
	cp->SetValue(PS_ALPHA, d2d::UICallback::Data(comp.alpha_start, comp.alpha_end));

	for (int i = 0, n = cp->m_sliders.size(); i < n; ++i) {
		cp->m_sliders[i]->Load();
	}

	if (!comp.bind_filepath.empty()) {
		pc->bind_ps_cfg = PSConfigMgr::Instance()->GetConfig(comp.bind_filepath);
	}

	m_comp_sizer->Insert(m_children.size(), cp);
	m_children.push_back(cp);

	this->Layout();
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(CreateMainLayout());
	sizer->Add(CreateComponentLayout());
	return sizer;
}

wxSizer* ToolbarPanel::CreateMainLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->AddSpacer(10);

	// Name
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, LANG[LK_NAME]));
		sizer->Add(m_name = new wxTextCtrl(this, wxID_ANY));
		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(10);
	// Capacity
	d2d::SliderCtrlOne* s_capacity = new d2d::SliderCtrlOne(this, LANG[LK_CAPACITY], 
		"capacity", this, PS_CAPACITY, d2d::SliderItem("", "", CAPACITY, 1, 1000));
	top_sizer->Add(s_capacity);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_capacity);
	// Count
	d2d::SliderCtrlOne* s_count = new d2d::SliderCtrlOne(this, LANG[LK_COUNT], 
		"count", this, PS_COUNT, d2d::SliderItem("", "", COUNT, 1, 100));
	top_sizer->Add(s_count);
	m_sliders.push_back(s_count);
	// Emission Time
	d2d::SliderCtrlOne* s_emission_time = new d2d::SliderCtrlOne(this, LANG[LK_EMISSION_TIME], 
		"emission_time", this, PS_EMISSION_TIME, d2d::SliderItem("", "", EMISSION_TIME, 10, 5000));
	top_sizer->Add(s_emission_time);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_emission_time);
	// Life
	d2d::SliderCtrlTwo* s_life = new d2d::SliderCtrlTwo(this, LANG[LK_LIFE], "life", this, PS_LIFE_TIME, 
		d2d::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, LIFE_CENTER, 0, 5000), d2d::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, LIFE_OFFSET, 0, 2500));
	top_sizer->Add(s_life);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_life);
	// Hori
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_HORI]);
		wxSizer* horiSizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, LANG[LK_MIN]));	

			m_min_hori = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 360, MIN_HORI);
			Connect(m_min_hori->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::OnSetHori));
			sizer->Add(m_min_hori);

			horiSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, LANG[LK_MAX]));	

			m_max_hori = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 360, MAX_HORI);
			Connect(m_max_hori->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::OnSetHori));
			sizer->Add(m_max_hori);

			horiSizer->Add(sizer);
		}
		top_sizer->Add(horiSizer);
	}
	top_sizer->AddSpacer(10);
	// Vert
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_VERT]);
		wxSizer* vertSizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, LANG[LK_MIN]));

			m_min_vert = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 90, MIN_VERT);
			Connect(m_min_vert->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::OnSetVert));
			sizer->Add(m_min_vert);

			vertSizer->Add(sizer);
		}
		vertSizer->AddSpacer(10);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, LANG[LK_MAX]));	

			m_max_vert = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 0, 90, MAX_VERT);
			Connect(m_max_vert->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::OnSetVert));
			sizer->Add(m_max_vert);

			vertSizer->Add(sizer);
		}
		top_sizer->Add(vertSizer);
	}
	top_sizer->AddSpacer(10);
	// Speed
	d2d::SliderCtrlTwo* s_spd = new d2d::SliderCtrlTwo(this, LANG[LK_LINEAR_SPEED], "speed", this, PS_SPEED, 
		d2d::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, SPEED_CENTER, 0, 9000), d2d::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, SPEED_OFFSET, 0, 4000));
	top_sizer->Add(s_spd);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_spd);
	// Angular Speed
	d2d::SliderCtrlTwo* s_aspd = new d2d::SliderCtrlTwo(this, LANG[LK_ANGULAR_SPEED], "angular_speed", this, PS_ANGULAR_SPEED, 
		d2d::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, ANGULAR_SPEED_CENTER, -3600, 3600), d2d::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, ANGULAR_SPEED_OFFSET, 0, 360));
	top_sizer->Add(s_aspd);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_aspd);
	// Disturbance 
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_DISTURBANCE]);
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		d2d::SliderCtrlTwo* s_dis_r = new d2d::SliderCtrlTwo(this, LANG[LK_RANGE], "disturbance_radius", this, PS_DISTURBANCE_RADIUS, 
			d2d::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, DISTURBANCE_RADIUS_CENTER, 0, 9999), d2d::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, DISTURBANCE_RADIUS_OFFSET, 0, 1000));
		sizer->Add(s_dis_r);
		m_sliders.push_back(s_dis_r);

		d2d::SliderCtrlTwo* s_dis_spd = new d2d::SliderCtrlTwo(this, LANG[LK_SPEED], "disturbance_spd", this, PS_DISTURBANCE_SPD, 
			d2d::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, DISTURBANCE_SPD_CENTER, 0, 9999), d2d::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, DISTURBANCE_SPD_OFFSET, 0, 1000));
		sizer->Add(s_dis_spd);
		m_sliders.push_back(s_dis_spd);

		top_sizer->Add(sizer);
		top_sizer->AddSpacer(10);
	}
	// Gravity
	d2d::SliderCtrlOne* s_gravity = new d2d::SliderCtrlOne(this, LANG[LK_GRAVITY], "gravity", 
		this, PS_GRAVITY, d2d::SliderItem("", "", GRAVITY, -5000, 25000));
	top_sizer->Add(s_gravity);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_gravity);
	// Linear Acc
	d2d::SliderCtrlTwo* s_lacc = new d2d::SliderCtrlTwo(this, LANG[LK_LINEAR_ACC], "linear_acc", this, PS_LINEAR_ACC, 
		d2d::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, LINEAR_ACC_CENTER, -9999, 9999), d2d::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, LINEAR_ACC_OFFSET, 0, 999));
	top_sizer->Add(s_lacc);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_lacc);
	// Fadeout Time
	d2d::SliderCtrlOne* s_fadeout = new d2d::SliderCtrlOne(this, LANG[LK_FADEOUT_TIME], 
		"fadeout_time", this, PS_FADEOUT_TIME, d2d::SliderItem("", "", FADEOUT_TIME, 10, 2500));
	top_sizer->Add(s_fadeout);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_fadeout);
	// Bounce
	{
		m_bounce = new wxCheckBox(this, wxID_ANY, LANG[LK_BOUNCE]);
		Connect(m_bounce->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetBounce));
		top_sizer->Add(m_bounce);
	}
	top_sizer->AddSpacer(10);
	// Start Radius
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_START_RADIUS]);
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_radius_3d = new wxCheckBox(this, wxID_ANY, "3D");
		Connect(m_radius_3d->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetRadius3D));
		sizer->Add(m_radius_3d);

		d2d::SliderCtrlOne* s_start_radius = new d2d::SliderCtrlOne(this, "", "start_radius", 
			this, PS_START_RADIUS, d2d::SliderItem("", "", START_RADIUS, 0, 1000));
		sizer->Add(s_start_radius);
		m_sliders.push_back(s_start_radius);

		top_sizer->Add(sizer);
		top_sizer->AddSpacer(10);
	}
	// orient_to_movement
	{
		m_orient_to_movement = new wxCheckBox(this, wxID_ANY, LANG[LK_ORIENT_MOVEMENT]);	
		Connect(m_orient_to_movement->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetOrientToMovement));
		top_sizer->Add(m_orient_to_movement);
	}
	top_sizer->AddSpacer(10);
	// 	// orient_to_parent
	// 	{
	// 		m_orient_to_parent = new wxCheckBox(this, wxID_ANY, LANG[LK_ORIENT_PARENT]);
	// 		leftSizer->Add(m_orient_to_parent);
	// 	}
	// 	leftSizer->AddSpacer(10);


	return top_sizer;
}

wxSizer* ToolbarPanel::CreateComponentLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->AddSpacer(10);
	// Remove All
	{
		wxButton* btn = new wxButton(this, wxID_ANY, LANG[LK_REMOVE_ALL]);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::OnDelAllChild));
		top_sizer->Add(btn);
		
	}
	top_sizer->AddSpacer(20);
	// Components
	{
		m_comp_sizer = new wxBoxSizer(wxVERTICAL);
		top_sizer->Add(m_comp_sizer);
	}
	return top_sizer;
}

void ToolbarPanel::InitParticle()
{
	clear();

	p3d_ps_config* cfg = PSConfigMgr::Instance()->GetDefaultConfig();
	ParticleSystem* ps = new ParticleSystem(PARTICLE_CAP, cfg);
//	ps->Start();
	d2d::obj_assign<ParticleSystem>(m_stage->m_ps, ps);
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}

	ps->SetHori(m_min_hori->GetValue(), m_max_hori->GetValue());
	ps->SetVert(m_min_vert->GetValue(), m_max_vert->GetValue());
}

void ToolbarPanel::OnDelChild(ComponentPanel* child)
{
	if (m_children.empty()) return;

	m_stage->m_ps->Clear();

	int idx = -1;
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i] == child) {
			idx = i;
			break;
		}
	}
	if (idx == -1) {
		return;
	}

	m_comp_sizer->Detach(idx);
	delete m_children[idx];
	m_children.erase(m_children.begin() + idx);

	m_stage->m_ps->DelSymbol(idx);

	this->Layout();	
}

void ToolbarPanel::clear()
{
	OnDelAllChild(wxCommandEvent());
}

void ToolbarPanel::OnAddChild(wxCommandEvent& event, d2d::ISymbol* symbol)
{
	p3d_symbol* ps = m_stage->m_ps->AddSymbol(symbol);
	ComponentPanel* cp = new ComponentPanel(this, ps, this);
	m_comp_sizer->Insert(m_children.size(), cp);
	m_comp_sizer->AddSpacer(10);
	m_children.push_back(cp);
	this->Layout();
}

void ToolbarPanel::OnDelAllChild(wxCommandEvent& event)
{
	if (m_children.empty()) {
		return;
	}

	m_stage->m_ps->Clear();

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_comp_sizer->Detach(m_children[i]);
		delete m_children[i];
	}
	m_children.clear();

	if (m_stage->m_ps) {
		m_stage->m_ps->DelAllSymbol();
	}

	Layout();
}

void ToolbarPanel::OnSetHori(wxSpinEvent& event)
{
	m_stage->m_ps->SetHori(m_min_hori->GetValue(), m_max_hori->GetValue());
}

void ToolbarPanel::OnSetVert(wxSpinEvent& event)
{
	m_stage->m_ps->SetVert(m_min_vert->GetValue(), m_max_vert->GetValue());
}

void ToolbarPanel::OnSetBounce(wxCommandEvent& event)
{
	m_stage->m_ps->SetBounce(event.IsChecked());
}

void ToolbarPanel::OnSetOrientToMovement(wxCommandEvent& event)
{
	m_stage->m_ps->SetOrientToMovement(event.IsChecked());
}

void ToolbarPanel::OnSetRadius3D(wxCommandEvent& event)
{
	m_stage->m_ps->SetRadius3D(event.IsChecked());
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
		m_toolbar->OnAddChild(wxCommandEvent(), symbol);

		m_stage->m_ps->Start();
		m_stage->ResetViewport();
	}

	return true;
}


}
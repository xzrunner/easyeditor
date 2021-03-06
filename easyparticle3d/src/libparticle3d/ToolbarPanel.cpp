#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "ps_config.h"
#include "PSConfigMgr.h"
#include "ComponentPanel.h"
#include "language.h"
#include "utility.h"

#include <ee/panel_msg.h>
#include <ee/SliderCtrl.h>
#include <ee/SymbolMgr.h>
#include <ee/LibraryPanel.h>
#include <ee/panel_msg.h>
#include <ee/subject_id.h>
#include <ee/SliderCtrlOne.h>
#include <ee/SliderCtrlTwo.h>
#include <ee/FastBlendModes.h>

#include <ps_3d.h>

#include <easyanim.h>

#include <wx/spinctrl.h>

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
static const float RADIAL_SPEED_CENTER	= 1600;
static const float RADIAL_SPEED_OFFSET	= 400;
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
static const float START_HEIGHT         = 0;

static const char* ITEM_ATTR_CENTER = "center";
static const char* ITEM_ATTR_OFFSET = "offset";

static const char* ITEM_ATTR_RADIUS = "radius";
static const char* ITEM_ATTR_HEIGHT = "height";

ToolbarPanel::ToolbarPanel(wxWindow* parent, ee::LibraryPanel* library,
						   StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
	, m_stage(stage)
	, m_image(NULL)
{
	SetScrollbars(1,1, 200, 100, 0, 0);
	SetSizer(InitLayout());	
	InitParticle();

	SetDropTarget(new DropTarget(library, stage, this));

	RegistSubject(ee::ClearPanelSJ::Instance());
}

void ToolbarPanel::SetValue(int key, const ee::UICallback::Data& data)
{
	if (m_stage->m_ps) {
		m_stage->m_ps->SetValue(key, data);
	}
}

void ToolbarPanel::GetValue(int key, ee::UICallback::Data& data)
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

void ToolbarPanel::Add(const s2loader::P3dSymLoader::Component& comp, ee::LibraryPanel* library)
{
	// todo Release symbol
	auto sym = ee::SymbolMgr::Instance()->FetchSymbol(comp.filepath.c_str());
	library->AddSymbol(sym);
	p3d_symbol* pc = m_stage->m_ps->AddSymbol(sym);
	ComponentPanel* cp = new ComponentPanel(this, pc, this);

	cp->SetCount(comp.count);

	cp->m_name->SetValue(comp.name.c_str());

	cp->SetValue(PS_SCALE, ee::UICallback::Data(comp.scale_start, comp.scale_end));

	cp->SetValue(PS_ROTATE, ee::UICallback::Data(comp.angle - comp.angle_var, comp.angle + comp.angle_var));

	pc->mul_col_begin = ColorFromS2(comp.mul_col_begin);
	pc->mul_col_end = ColorFromS2(comp.mul_col_end);
	pc->add_col_begin = ColorFromS2(comp.add_col_begin);
	pc->add_col_end = ColorFromS2(comp.add_col_end);

	cp->SetValue(PS_ALPHA, ee::UICallback::Data(comp.alpha_start, comp.alpha_end));

	cp->UpdateBtnColor();

	for (int i = 0, n = cp->m_sliders.size(); i < n; ++i) {
		cp->m_sliders[i]->Load();
	}

	m_comp_sizer->Insert(m_children.size(), cp);
	m_children.push_back(cp);

	this->Layout();
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(CreateMainLayout());
	sizer->Add(CreateComponentLayout());
	return sizer;
}

void ToolbarPanel::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case ee::MSG_CLEAR_PANEL:
		Clear();
		break;
	}
}

wxSizer* ToolbarPanel::CreateMainLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->AddSpacer(10);

// 	// Name
// 	{
// 		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
// 		sizer->Add(new wxStaticText(this, wxID_ANY, LANG[LK_NAME]));
// 		sizer->Add(m_name = new wxTextCtrl(this, wxID_ANY));
// 		top_sizer->Add(sizer);
// 	}
// 	// State
// 	{
// 		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_STATE]);
// 		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
// 		{
// 			m_loop = new wxCheckBox(this, wxID_ANY, LANG[LK_LOOP]);	
// 			m_loop->SetValue(true);
// 			Connect(m_loop->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetLoop));
// 			sizer->Add(m_loop);
// 		}
// 		sizer->AddSpacer(5);
// 		{
// 			m_local_mode_draw = new wxCheckBox(this, wxID_ANY, LANG[LK_LOCAL_DRAW]);	
// 			m_local_mode_draw->SetValue(false);
// 			Connect(m_local_mode_draw->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetLocalModeDraw));
// 			sizer->Add(m_local_mode_draw);
// 		}
// 		top_sizer->Add(sizer);
// 	}
// 	top_sizer->AddSpacer(10);

	// Mode
	m_static_mode = new wxCheckBox(this, wxID_ANY, LANG[LK_STATIC_MODE]);
	Connect(m_static_mode->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetStaticMode));
	top_sizer->Add(m_static_mode);

	// Count
	ee::SliderCtrlOne* s_count = new ee::SliderCtrlOne(this, LANG[LK_COUNT], 
		"count", this, PS_COUNT, ee::SliderItem("", "", COUNT, 1, 5000));
	top_sizer->Add(s_count);
	m_sliders.push_back(s_count);
	m_count_ctrl = s_count;
	// Emission Time
	ee::SliderCtrlOne* s_emission_time = new ee::SliderCtrlOne(this, LANG[LK_EMISSION_TIME], 
		"emission_time", this, PS_EMISSION_TIME, ee::SliderItem("", "", EMISSION_TIME, 10, 5000));
	top_sizer->Add(s_emission_time);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_emission_time);
	m_time_ctrl = s_emission_time;

	// Life
	ee::SliderCtrlTwo* s_life = new ee::SliderCtrlTwo(this, LANG[LK_LIFE], "life", this, PS_LIFE_TIME, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, LIFE_CENTER, 0, 5000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, LIFE_OFFSET, 0, 2500));
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

			m_min_hori = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, -360, 360, MIN_HORI);
			Connect(m_min_hori->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::OnSetHori));
			sizer->Add(m_min_hori);

			horiSizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, LANG[LK_MAX]));	

			m_max_hori = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, -360, 360, MAX_HORI);
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

			m_min_vert = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, -360, 360, MIN_VERT);
			Connect(m_min_vert->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::OnSetVert));
			sizer->Add(m_min_vert);

			vertSizer->Add(sizer);
		}
		vertSizer->AddSpacer(10);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, LANG[LK_MAX]));	

			m_max_vert = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, -360, 360, MAX_VERT);
			Connect(m_max_vert->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::OnSetVert));
			sizer->Add(m_max_vert);

			vertSizer->Add(sizer);
		}
		top_sizer->Add(vertSizer);
	}
	top_sizer->AddSpacer(10);
	// Radial Speed
	ee::SliderCtrlTwo* s_r_spd = new ee::SliderCtrlTwo(this, LANG[LK_RADIAL_SPEED], "radial_speed", this, PS_RADIAL_SPEED, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, RADIAL_SPEED_CENTER, -40000, 40000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, RADIAL_SPEED_OFFSET, 0, 20000));
	top_sizer->Add(s_r_spd);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_r_spd);
	// Tangential Speed
	ee::SliderCtrlTwo* s_t_spd = new ee::SliderCtrlTwo(this, LANG[LK_TANGENTIAL_SPEED], "tangential_spd", this, PS_TANGENTIAL_SPEED, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, 0, -400, 400), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 200));
	top_sizer->Add(s_t_spd);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_t_spd);
	// Angular Speed
	ee::SliderCtrlTwo* s_a_spd = new ee::SliderCtrlTwo(this, LANG[LK_ANGULAR_SPEED], "angular_speed", this, PS_ANGULAR_SPEED, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, ANGULAR_SPEED_CENTER, -3600, 3600), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, ANGULAR_SPEED_OFFSET, 0, 360));
	top_sizer->Add(s_a_spd);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_a_spd);
	// Disturbance 
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_DISTURBANCE]);
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		ee::SliderCtrlTwo* s_dis_r = new ee::SliderCtrlTwo(this, LANG[LK_RANGE], "disturbance_radius", this, PS_DISTURBANCE_RADIUS, 
			ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, DISTURBANCE_RADIUS_CENTER, 0, 9999), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, DISTURBANCE_RADIUS_OFFSET, 0, 1000));
		sizer->Add(s_dis_r);
		m_sliders.push_back(s_dis_r);

		ee::SliderCtrlTwo* s_dis_spd = new ee::SliderCtrlTwo(this, LANG[LK_SPEED], "disturbance_spd", this, PS_DISTURBANCE_SPD, 
			ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, DISTURBANCE_SPD_CENTER, 0, 9999), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, DISTURBANCE_SPD_OFFSET, 0, 1000));
		sizer->Add(s_dis_spd);
		m_sliders.push_back(s_dis_spd);

		top_sizer->Add(sizer);
		top_sizer->AddSpacer(10);
	}
	// Gravity
	ee::SliderCtrlOne* s_gravity = new ee::SliderCtrlOne(this, LANG[LK_GRAVITY], "gravity", 
		this, PS_GRAVITY, ee::SliderItem("", "", GRAVITY, -5000, 25000));
	top_sizer->Add(s_gravity);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_gravity);
	// Linear Acc
	ee::SliderCtrlTwo* s_lacc = new ee::SliderCtrlTwo(this, LANG[LK_LINEAR_ACC], "linear_acc", this, PS_LINEAR_ACC, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, LINEAR_ACC_CENTER, -9999, 9999), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, LINEAR_ACC_OFFSET, 0, 999));
	top_sizer->Add(s_lacc);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_lacc);
	// Fadeout Time
	ee::SliderCtrlOne* s_fadeout = new ee::SliderCtrlOne(this, LANG[LK_FADEOUT_TIME], 
		"fadeout_time", this, PS_FADEOUT_TIME, ee::SliderItem("", "", FADEOUT_TIME, 10, 2500));
	top_sizer->Add(s_fadeout);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_fadeout);
	// Ground
	{
		const int size = 3;
		wxString choices[size];
		choices[0] = LANG[LK_NO_GROUND];
		choices[1] = LANG[LK_GROUND_WITH_BOUNCE];
		choices[2] = LANG[LK_GROUND_NO_BOUNCE];
		m_ground = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, size, choices);
		m_ground->SetSelection(1);
		Connect(m_ground->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ToolbarPanel::OnSetGround));
		top_sizer->Add(m_ground);
	}
	top_sizer->AddSpacer(10);
	// Start Position
	ee::SliderCtrlTwo* s_start_pos = new ee::SliderCtrlTwo(this, LANG[LK_START_POSITION], "start_pos", this, PS_START_POS, 
		ee::SliderItem(LANG[LK_RADIUS], ITEM_ATTR_RADIUS, START_RADIUS, 0, 5000), 
		ee::SliderItem(LANG[LK_HEIGHT], ITEM_ATTR_HEIGHT, START_HEIGHT, -25000, 25000));
	top_sizer->Add(s_start_pos);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_start_pos);
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

	// Blend
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(this, wxID_ANY, "���"));

		std::vector<std::string> names;
		ee::FastBlendModes::Instance()->GetAllNameCN(names);
		wxArrayString wx_names;
		for (int i = 0, n = names.size(); i < n; ++i) {
			wx_names.Add(names[i]);
		}
		m_blend = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wx_names);
		m_blend->SetSelection(0);
		Connect(m_blend->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ToolbarPanel::OnSetBlend));
		sizer->Add(m_blend);

		top_sizer->Add(sizer);
	}

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
	Clear();

	auto cfg = PSConfigMgr::Instance()->GetDefaultConfig();
	ParticleSystem* ps = new ParticleSystem(cfg, false);
//	ps->Start();
	cu::RefCountObjAssign<ParticleSystem>(m_stage->m_ps, ps);
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}

	ps->SetHori(m_min_hori->GetValue(), m_max_hori->GetValue());
	ps->SetVert(m_min_vert->GetValue(), m_max_vert->GetValue());
	ps->SetGround(m_ground->GetSelection());
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

	m_comp_sizer->Detach(m_children[idx]);
	delete m_children[idx];
	m_children.erase(m_children.begin() + idx);

	m_stage->m_ps->DelSymbol(idx);

	this->Layout();	

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->m_pc = m_stage->m_ps->GetSymbol(i);
	}
}

void ToolbarPanel::OnSetStaticMode(bool static_mode)
{
	m_static_mode->SetValue(static_mode);
	m_count_ctrl->Enable(!static_mode);
	m_time_ctrl->Enable(!static_mode);
	m_stage->m_ps->SetStaticMode(static_mode);
}

void ToolbarPanel::Clear()
{
	OnDelAllChild(wxCommandEvent());
}

void ToolbarPanel::OnAddChild(wxCommandEvent& event, const s2::SymPtr& sym)
{
	p3d_symbol* ps = m_stage->m_ps->AddSymbol(sym);
	ComponentPanel* cp = new ComponentPanel(this, ps, this);
	m_comp_sizer->Insert(m_children.size(), cp);
	m_comp_sizer->AddSpacer(10);
	m_children.push_back(cp);
	this->Layout();
}

void ToolbarPanel::OnSetStaticMode(wxCommandEvent& event)
{
	bool static_mode = event.IsChecked();
	OnSetStaticMode(static_mode);
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

void ToolbarPanel::OnSetLoop(wxCommandEvent& event)
{
	m_stage->m_ps->SetLoop(event.IsChecked());
}

void ToolbarPanel::OnSetLocalModeDraw(wxCommandEvent& event)
{
	m_stage->m_ps->SetLocal(event.IsChecked());
}

void ToolbarPanel::OnSetHori(wxSpinEvent& event)
{
	m_stage->m_ps->SetHori(m_min_hori->GetValue(), m_max_hori->GetValue());
}

void ToolbarPanel::OnSetVert(wxSpinEvent& event)
{
	m_stage->m_ps->SetVert(m_min_vert->GetValue(), m_max_vert->GetValue());
}

void ToolbarPanel::OnSetGround(wxCommandEvent& event)
{
	int ground = m_ground->GetSelection();
	m_stage->m_ps->SetGround(ground);
}

void ToolbarPanel::OnSetOrientToMovement(wxCommandEvent& event)
{
	m_stage->m_ps->SetOrientToMovement(event.IsChecked());
}

void ToolbarPanel::OnSetBlend(wxCommandEvent& event)
{
	int blend = m_blend->GetSelection();
	m_stage->m_ps->SetBlend(blend);
}

//////////////////////////////////////////////////////////////////////////
// class ToolbarPanel::DropTarget
//////////////////////////////////////////////////////////////////////////

ToolbarPanel::DropTarget::
DropTarget(ee::LibraryPanel* library, StagePanel* stage,
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

	auto sym = m_library->GetSymbol(index);
	if (sym)
	{
		m_toolbar->OnAddChild(wxCommandEvent(), sym);
		m_stage->m_ps->Start();
	}

	return true;
}


}
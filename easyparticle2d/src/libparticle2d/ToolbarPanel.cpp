#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "language.h"
#include "ps_config.h"
#include "item_string.h"
#include "ParticleSystem.h"
#include "PSConfigMgr.h"
#include "ComponentPanel.h"

#include <ee/SymbolMgr.h>
#include <ee/SliderCtrlOne.h>
#include <ee/SliderCtrlTwo.h>
#include <ee/LibraryPanel.h>
#include <ee/Symbol.h>

#include <ps_2d.h>

namespace eparticle2d
{

static const float CAPACITY				= 100;
static const float COUNT				= 6;
static const float EMISSION_TIME		= 100;
static const float LIFE_CENTER			= 800;
static const float LIFE_OFFSET			= 500;
static const float X_OFFSET				= 60;
static const float ANGLE_CENTER			= 90;
static const float SPEED_CENTER			= 120;
static const float RADIAL_ACCEL_CENTER	= 700;

ToolbarPanel::ToolbarPanel(wxWindow* parent, ee::LibraryPanel* library, 
							 StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
	, m_stage(stage)
{
	SetScrollbars(1,1, 200, 100, 0, 0);
	SetSizer(InitLayout());	
	InitParticle();

	SetDropTarget(new DropTarget(library, stage, this));
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

void ToolbarPanel::Load(const Json::Value& val)
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Load(val, 0);
		m_sliders[i]->Update();
	}

	int mode_type = val["mode_type"].asInt();
	for (int i = 0, n = m_mode_sliders[mode_type].size(); i < n; ++i) {
		m_mode_sliders[mode_type][i]->Load(val, 0);
		m_mode_sliders[mode_type][i]->Update();
	}
}

void ToolbarPanel::Store(Json::Value& val) const
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Store(val);
	}

	int mode_type = m_mode_choice->GetSelection();
	for (int i = 0, n = m_mode_sliders[mode_type].size(); i < n; ++i) {
		m_mode_sliders[mode_type][i]->Store(val);
	}
}

inline ps_color ColorFromS2(const s2::Color& col)
{
	ps_color ret;
	ret.r = col.r;
	ret.g = col.g;
	ret.b = col.b;
	ret.a = col.a;
	return ret;
}

void ToolbarPanel::Add(const LoadAdapter::Component& comp)
{
	// todo Release symbol
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(comp.filepath);
	p2d_symbol* pc = m_stage->m_ps->AddSymbol(static_cast<s2::Symbol*>(sym));
	ComponentPanel* cp = new ComponentPanel(this, pc, this);

	cp->SetValue(PS_ANGLE, ee::UICallback::Data(comp.angle_start, comp.angle_end));

	cp->SetValue(PS_SCALE, ee::UICallback::Data(comp.scale_start, comp.scale_end));

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

void ToolbarPanel::InitParticle()
{
	Clear();

	p2d_emitter_cfg* cfg = PSConfigMgr::Instance()->GetDefaultConfig();
	ParticleSystem* ps = new ParticleSystem(cfg);
	//	ps->Start();
	cu::RefCountObjAssign<ParticleSystem>(m_stage->m_ps, ps);
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}

	int mode_type = cfg->mode_type;
	for (int i = 0, n = m_mode_sliders[mode_type].size(); i < n; ++i) {
		m_mode_sliders[mode_type][i]->Update();
	}
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

wxSizer* ToolbarPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_main_sizer = CreateMainLayout());
	sizer->Add(CreateComponentLayout());
	return sizer;
}

wxSizer* ToolbarPanel::CreateMainLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->Add(CreateMainCommonLayout());

	top_sizer->Add(CreateModeGravityLayout());
	top_sizer->Add(CreateModeRadiusLayout());
	top_sizer->Add(CreateModeSpdCosLayout());

	top_sizer->Hide(2);
	top_sizer->Hide(3);

	return top_sizer;
}

wxSizer* ToolbarPanel::CreateMainCommonLayout()
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
	// State
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_STATE]);
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		{
			m_loop = new wxCheckBox(this, wxID_ANY, LANG[LK_LOOP]);	
			m_loop->SetValue(true);
			Connect(m_loop->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetLoop));
			sizer->Add(m_loop);
		}
		sizer->AddSpacer(5);
		{
			m_local_mode_draw = new wxCheckBox(this, wxID_ANY, LANG[LK_LOCAL_DRAW]);	
			m_local_mode_draw->SetValue(false);
			Connect(m_local_mode_draw->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetLocalModeDraw));
			sizer->Add(m_local_mode_draw);
		}
		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(10);
	// Count
	ee::SliderCtrlOne* s_count = new ee::SliderCtrlOne(this, LANG[LK_COUNT], 
		"count", this, PS_COUNT, ee::SliderItem("", "", COUNT, 1, 100));
	top_sizer->Add(s_count);
	m_sliders.push_back(s_count); 
	// Emission Time
	ee::SliderCtrlOne* s_emission_time = new ee::SliderCtrlOne(this, LANG[LK_EMISSION_TIME], 
		"emission_time", this, PS_EMISSION_TIME, ee::SliderItem("", "", EMISSION_TIME, 10, 5000));
	top_sizer->Add(s_emission_time);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_emission_time);
	// Life
	ee::SliderCtrlTwo* s_flie = new ee::SliderCtrlTwo(this, LANG[LK_LIFE], "life", this, PS_LIFE_TIME, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, LIFE_CENTER, 0, 5000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, LIFE_OFFSET, 0, 2500));
	top_sizer->Add(s_flie);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_flie);
	// Position
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_POSITION]);
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		ee::SliderCtrlTwo* s_pos_x = new ee::SliderCtrlTwo(this, LANG[LK_X], "position_x", this, PS_POSITION_X, 
			ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, 0, -500, 500), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, X_OFFSET, 0, 1000));
		sizer->Add(s_pos_x);
		m_sliders.push_back(s_pos_x);

		ee::SliderCtrlTwo* s_pos_y = new ee::SliderCtrlTwo(this, LANG[LK_Y], "position_y", this, PS_POSITION_Y, 
			ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, 0, -500, 500), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 1000));
		sizer->Add(s_pos_y);
		m_sliders.push_back(s_pos_y);

		top_sizer->Add(sizer);
		top_sizer->AddSpacer(10);
	}
	// Direction
	ee::SliderCtrlTwo* s_direction = new ee::SliderCtrlTwo(this, LANG[LK_DIRECTION], "direction", this, PS_DIRECTION, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, ANGLE_CENTER, 0, 360), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 90));
	top_sizer->Add(s_direction);
	top_sizer->AddSpacer(20);
	m_sliders.push_back(s_direction);
	// Mode
	wxArrayString modes_str;
	modes_str.push_back(LANG[LK_MODE_GRAVITY]);
	modes_str.push_back(LANG[LK_MODE_RADIUS]);
	modes_str.push_back(LANG[LK_MODE_SPD_COS]);
	m_mode_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, modes_str);
	m_mode_choice->SetSelection(0);
	Connect(m_mode_choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ToolbarPanel::OnChangeMode));
	top_sizer->Add(m_mode_choice);
	top_sizer->AddSpacer(10);

	return top_sizer;
}

wxSizer* ToolbarPanel::CreateModeGravityLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	std::vector<ee::SliderCtrl*> sliders;

	// Gravity
	ee::SliderCtrlTwo* s_gravity = new ee::SliderCtrlTwo(this, LANG[LK_GRAVITY], "gravity", this, PS_GRAVITY, 
		ee::SliderItem(LANG[LK_X], ITEM_ATTR_X, 0, 0, 1000), ee::SliderItem(LANG[LK_Y], ITEM_ATTR_Y, 0, 0, 1000));
	top_sizer->Add(s_gravity);
	top_sizer->AddSpacer(10);
	sliders.push_back(s_gravity);
	// Speed
	ee::SliderCtrlTwo* s_speed = new ee::SliderCtrlTwo(this, LANG[LK_SPEED], "speed", this, PS_SPEED, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, SPEED_CENTER, 0, 1000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 1000));
	top_sizer->Add(s_speed);
	top_sizer->AddSpacer(10);
	sliders.push_back(s_speed);
	// tangential accel
	ee::SliderCtrlTwo* s_tangential = new ee::SliderCtrlTwo(this, LANG[LK_TANGENTIAL_ACCEL], "tangential_accel", this, PS_TANGENTIAL_ACCEL, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, 0, 0, 1000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 1000));
	top_sizer->Add(s_tangential);
	top_sizer->AddSpacer(10);
	sliders.push_back(s_tangential);
	// radial accel
	ee::SliderCtrlTwo* s_radial = new ee::SliderCtrlTwo(this, LANG[LK_RADIAL_ACCEL], "radial_accel", this, PS_RADIAL_ACCEL, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, RADIAL_ACCEL_CENTER, 0, 1000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 1000));
	top_sizer->Add(s_radial);
	top_sizer->AddSpacer(10);
	sliders.push_back(s_radial);
	// rotation is dir
	m_rotation_is_dir_ctrl = new wxCheckBox(this, wxID_ANY, LANG[LK_ROTATIOIN_IS_DIR]);
	Connect(m_rotation_is_dir_ctrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ToolbarPanel::OnSetRotationIsDir));
	top_sizer->Add(m_rotation_is_dir_ctrl);

	m_mode_sliders.push_back(sliders);
	return top_sizer;
}

wxSizer* ToolbarPanel::CreateModeRadiusLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	std::vector<ee::SliderCtrl*> sliders;

	// Start Radius
	ee::SliderCtrlTwo* s_start_radius = new ee::SliderCtrlTwo(this, LANG[LK_START_RADIUS], "start_radius", this, PS_START_RADIUS, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, 0, 0, 1000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 1000));
	top_sizer->Add(s_start_radius);
	top_sizer->AddSpacer(10);
	sliders.push_back(s_start_radius);
	// End Radius
	ee::SliderCtrlTwo* s_end_radius = new ee::SliderCtrlTwo(this, LANG[LK_END_RADIUS], "end_radius", this, PS_END_RADIUS, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, 0, 0, 1000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 1000));
	top_sizer->Add(s_end_radius);
	top_sizer->AddSpacer(10);
	sliders.push_back(s_end_radius);
	// Direction Delta
	ee::SliderCtrlTwo* s_direction_spd = new ee::SliderCtrlTwo(this, LANG[LK_DIRECTION_SPEED], "direction_delta", this, PS_DIRECTION_SPEED, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, 0, 0, 1000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 1000));
	top_sizer->Add(s_direction_spd);
	top_sizer->AddSpacer(10);
	sliders.push_back(s_direction_spd);

	m_mode_sliders.push_back(sliders);
	return top_sizer;
}

wxSizer* ToolbarPanel::CreateModeSpdCosLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	std::vector<ee::SliderCtrl*> sliders;

	// Speed
	ee::SliderCtrlTwo* s_speed = new ee::SliderCtrlTwo(this, LANG[LK_SPEED], "speed", this, PS_SPEED, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, 0, 0, 1000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 1000));
	top_sizer->Add(s_speed);
	top_sizer->AddSpacer(10);
	sliders.push_back(s_speed);
	// Cos Amplitude
	ee::SliderCtrlTwo* s_cos_amplitude = new ee::SliderCtrlTwo(this, LANG[LK_SPD_COS_AMPLITUDE], "cos_amplitude", this, PS_COS_AMPLITUDE, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, 0, 0, 1000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 1000));
	top_sizer->Add(s_cos_amplitude);
	top_sizer->AddSpacer(10);
	sliders.push_back(s_cos_amplitude);
	// Cos Frequency
	ee::SliderCtrlTwo* s_cos_frequency = new ee::SliderCtrlTwo(this, LANG[LK_SPD_COS_FREQUENCY], "cos_frequency", this, PS_COS_FREQUENCY, 
		ee::SliderItem(LANG[LK_CENTER], ITEM_ATTR_CENTER, 0, 0, 1000), ee::SliderItem(LANG[LK_OFFSET], ITEM_ATTR_OFFSET, 0, 0, 1000));
	top_sizer->Add(s_cos_frequency);
	top_sizer->AddSpacer(10);
	sliders.push_back(s_cos_frequency);

	m_mode_sliders.push_back(sliders);
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

void ToolbarPanel::OnSetLoop(wxCommandEvent& event)
{
	m_stage->m_ps->SetLoop(event.IsChecked());
}

void ToolbarPanel::OnSetLocalModeDraw(wxCommandEvent& event)
{
	m_stage->m_ps->SetLocalModeDraw(event.IsChecked());
}

void ToolbarPanel::OnChangeMode(wxCommandEvent& event)
{
	int idx = m_mode_choice->GetSelection();
	if (idx == 0) {
		m_main_sizer->Show((size_t)1);
		m_main_sizer->Hide(2);
		m_main_sizer->Hide(3);
	} else if (idx == 1) {
		m_main_sizer->Hide(1);
		m_main_sizer->Show((size_t)2);
		m_main_sizer->Hide(3);
	} else if (idx == 2) {
		m_main_sizer->Hide(1);
		m_main_sizer->Hide(2);
		m_main_sizer->Show((size_t)3);
	}

	Layout();
	
	if (m_stage->m_ps) {
		m_stage->m_ps->SetMode(idx);
	}
}

void ToolbarPanel::OnSetRotationIsDir(wxCommandEvent& event)
{
	// todo
//	m_stage->m_ps->SetBounce(event.IsChecked());
}

void ToolbarPanel::Clear()
{
	OnDelAllChild(wxCommandEvent());
}

void ToolbarPanel::OnAddChild(wxCommandEvent& event, s2::Symbol* sym)
{
	p2d_symbol* p_symbol = m_stage->m_ps->AddSymbol(sym);
	ComponentPanel* cp = new ComponentPanel(this, p_symbol, this);
	m_comp_sizer->Insert(m_children.size(), cp);
	m_comp_sizer->AddSpacer(10);
	m_children.push_back(cp);
	Layout();
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

	ee::Symbol* sym = m_library->GetSymbol(index);
	if (sym)
	{
		m_toolbar->OnAddChild(wxCommandEvent(), static_cast<s2::Symbol*>(sym));
		m_stage->m_ps->Start();
	}

	return true;
}

}
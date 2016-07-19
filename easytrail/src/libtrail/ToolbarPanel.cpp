#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "MTConfigMgr.h"
#include "MotionTrail.h"
#include "ComponentPanel.h"
#include "mt_config.h"

#include <ee/panel_msg.h>
#include <ee/SliderCtrl.h>
#include <ee/LibraryPanel.h>
#include <ee/SliderCtrlOne.h>

#include <mt_2d.h>

namespace etrail
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, ee::LibraryPanel* library,
						   StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
	, m_stage(stage)
{
	SetScrollbars(1,1, 200, 100, 0, 0);
	SetSizer(InitLayout());	
	InitTrail();

	SetDropTarget(new DropTarget(library, stage, this));

	RegistSubject(ee::ClearPanelSJ::Instance());
}

void ToolbarPanel::SetValue(int key, const ee::UICallback::Data& data)
{
	if (m_stage->m_trail) {
		m_stage->m_trail->SetValue(key, data);
	}
}

void ToolbarPanel::GetValue(int key, ee::UICallback::Data& data)
{
	if (m_stage->m_trail) {
		m_stage->m_trail->GetValue(key, data);
	}	
}

void ToolbarPanel::Load(const Json::Value& val, int version)
{

}

void ToolbarPanel::Store(Json::Value& val) const
{

}

// void ToolbarPanel::Add(const LoadAdapter::Component& comp, ee::LibraryPanel* library)
// {
// 
// }

void ToolbarPanel::InitTrail()
{
	Clear();

	t2d_emitter_cfg* cfg = MTConfigMgr::Instance()->GetDefaultConfig();
	MotionTrail* mt = new MotionTrail(cfg);
	//	mt->Start();
	ee::obj_assign<MotionTrail>(m_stage->m_trail, mt);
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}
}

void ToolbarPanel::OnDelChild(ComponentPanel* child)
{
	if (m_children.empty()) return;

	m_stage->m_trail->Clear();

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

	m_stage->m_trail->DelSymbol(idx);

	this->Layout();	

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->SetSymbol(m_stage->m_trail->GetSymbol(i));
	}
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(CreateMainLayout());
	sizer->AddSpacer(10);
	sizer->Add(CreateComponentLayout());
	return sizer;
}

void ToolbarPanel::OnNotify(int sj_id, void* ud)
{

}

wxSizer* ToolbarPanel::CreateMainLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->AddSpacer(10);

	// Count
	ee::SliderCtrlOne* s_count = new ee::SliderCtrlOne(this, "数量", 
		"count", this, MT_COUNT, ee::SliderItem("", "", 20, 1, 500));
	top_sizer->Add(s_count);
	m_sliders.push_back(s_count);

	top_sizer->AddSpacer(10);

	// Life Time Begin
	ee::SliderCtrlOne* s_life_begin = new ee::SliderCtrlOne(this, "起点生命周期(ms)", 
		"life begin", this, MT_LIFETIME_BEGIN, ee::SliderItem("", "", 1000, -1, 5000));
	top_sizer->Add(s_life_begin);
	m_sliders.push_back(s_life_begin);
	// Life Time Offset
	ee::SliderCtrlOne* s_life_offset = new ee::SliderCtrlOne(this, "生命周期增量(ms)", 
		"life offset", this, MT_LIFETIME_OFFSET, ee::SliderItem("", "", 0, 0, 1000));
	top_sizer->Add(s_life_offset);
	m_sliders.push_back(s_life_offset);

	top_sizer->AddSpacer(10);

	// Fadeout Time
	ee::SliderCtrlOne* s_fadeout = new ee::SliderCtrlOne(this, "消逝时间", 
		"fadeout_time", this, MT_FADEOUT_TIME, ee::SliderItem("", "", 0, 0, 2500));
	top_sizer->Add(s_fadeout);
	m_sliders.push_back(s_fadeout);

	return top_sizer;
}

wxSizer* ToolbarPanel::CreateComponentLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->AddSpacer(10);
	// Remove All
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "删除全部");
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

void ToolbarPanel::Clear()
{
	OnDelAllChild(wxCommandEvent());
}

void ToolbarPanel::OnAddChild(wxCommandEvent& event, ee::Symbol* symbol)
{
	t2d_symbol* mt = m_stage->m_trail->AddSymbol(symbol);
	ComponentPanel* cp = new ComponentPanel(this, mt, this);
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

	if (m_stage->m_trail) {
		m_stage->m_trail->Clear();
	}

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_comp_sizer->Detach(m_children[i]);
		delete m_children[i];
	}
	m_children.clear();

	if (m_stage->m_trail) {
		m_stage->m_trail->DelAllSymbol();
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

	ee::Symbol* symbol = m_library->GetSymbol(index);
	if (symbol)
	{
		m_toolbar->OnAddChild(wxCommandEvent(), symbol);
//		m_stage->m_ps->Start();
	}

	return true;
}

}
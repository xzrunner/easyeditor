#include "ToolBarPanel.h"
#include "StageData.h"
#include "EmitterSettingPage.h"
#include "SpecialSettingPage.h"

namespace eparticle2d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library,
						   d2d::EditPanel* stage, StageData* stage_data)
	: d2d::ToolbarPanel(parent, stage)
	, m_stage_data(stage_data)
{
	m_stage_data->SetToolbar(this);

	SetScrollbars(1,1, 200, 100, 0, 0);
	SetSizer(initLayout());	

	SetDropTarget(new DropTarget(library, stage_data));

	InitPSFromUI();
}

void ToolbarPanel::Load(const Json::Value& val, int version)
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		m_pages[i]->Load(val, version);
	}
}

void ToolbarPanel::Store(Json::Value& val) const
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		m_pages[i]->Store(val);
	}
}

void ToolbarPanel::ResetUIFromPS()
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		m_pages[i]->ResetUIFromPS();
	}
}

void ToolbarPanel::InitPSFromUI()
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		m_pages[i]->InitPSFromUI();
	}
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	m_notebook = new wxNotebook(this, wxID_ANY);
	sizer->Add(m_notebook, 1, wxEXPAND);

	SettingPageBase* emitter_page = new EmitterSettingPage(m_notebook, m_stage_data);
	m_notebook->AddPage(emitter_page, "Emitter");
	m_pages.push_back(emitter_page);

	SettingPageBase* special_page = new SpecialSettingPage(m_notebook, m_stage_data);
	m_notebook->AddPage(special_page, "special");
	m_pages.push_back(special_page);

	return sizer;
}

//////////////////////////////////////////////////////////////////////////
// class ToolbarPanel::DropTarget
//////////////////////////////////////////////////////////////////////////

ToolbarPanel::DropTarget::
DropTarget(d2d::LibraryPanel* library, StageData* stage_data)
	: m_library(library)
	, m_stage_data(stage_data)
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
	if (symbol) {
		m_stage_data->ChangePSSymbol(symbol);
	}

	return true;
}

}
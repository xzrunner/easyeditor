#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/PropertySettingPanel.h>
#include <ee/SymbolPropertySetting.h>

#include <easytrail.h>

namespace etrail
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_library(NULL)
	, m_property(NULL)
	, m_stage(NULL)
{
	InitLayout();

	m_property->SetPropertySetting(
		new ee::SymbolPropertySetting(&m_stage->m_trail->name, NULL));
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filepath)
{
	FileIO::Load(filepath, m_stage->m_trail, m_toolbar, m_library);
	m_property->SetPropertySetting(
		new ee::SymbolPropertySetting(&m_stage->m_trail->name, NULL));
}

void Task::Store(const char* filepath) const
{
	FileIO::Store(filepath, m_stage->m_trail, m_toolbar);
}

bool Task::IsDirty() const
{
	return false;
}

const ee::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::InitLayout()
{
	wxSplitterWindow* right_split = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split);
	wxWindow* right = InitLayoutRight(right_split);

	left_split->SetSashGravity(0.2f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.7f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_library = new LibraryPanel(split);

	m_property = new PropertySettingPanel(split);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	StagePanel* stage = new StagePanel(parent, m_parent, m_library, m_property);
	m_stage = stage;
	m_property->SetEditPanel(stage->GetStageImpl());
	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	m_toolbar = new ToolbarPanel(parent, m_library, static_cast<StagePanel*>(m_stage));
	return m_toolbar;
}

}
#include "Task.h"

#include <easytemplate.h>

namespace etemplate
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_library(NULL)
	, m_property(NULL)
	, m_stage(NULL)
{
	InitLayout();
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;
}

void Task::Load(const char* filename)
{
}

void Task::Store(const char* filename) const
{
}

bool Task::IsDirty() const
{
	return false;
}

void Task::Clear()
{
	m_library->Clear();
	m_stage->clear();
	m_library->Refresh();
	m_stage->Refresh();
}

const d2d::EditPanel* Task::GetEditPanel() const
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

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_library = new LibraryPanel(split);

	m_property = new d2d::PropertySettingPanel(split);
	m_view_panel_mgr.AddSpritePanel(m_property);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	StagePanel* stage = new StagePanel(parent, m_parent, m_library, m_property, &m_view_panel_mgr);
	m_view_panel_mgr.AddSpritePanel(stage);
	m_stage = stage;
	m_library->SetCanvas(m_stage->getCanvas());	
	m_property->SetEditPanel(m_stage);
	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	ToolbarPanel* toolbar = new ToolbarPanel(parent, static_cast<StagePanel*>(m_stage));
	return toolbar;
}

}
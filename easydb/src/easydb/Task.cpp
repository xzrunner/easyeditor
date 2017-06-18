#include "Task.h"
#include "StagePanel.h"
#include "MainPanel.h"
#include "MainList.h"
#include "SearchPanel.h"
#include "WarnPanel.h"
#include "WarnList.h"
#include "Database.h"

#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/FileHelper.h>
#include <ee/panel_msg.h>

#include <wx/splitter.h>

namespace edb
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	InitLayout();
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	delete m_root;
}

void Task::LoadFromFile(const char* filename)
{
	if (!ee::FileHelper::IsFileExist(filename)) {
		throw ee::Exception("File: %s don't exist!", filename);
	}

	m_stage_panel->GetDB()->Load(filename);
	m_main_panel->GetList()->Build(*m_stage_panel->GetDB());
	m_warn_panel->GetList()->Build(*m_stage_panel->GetDB());
}

void Task::StoreToFile(const char* filename) const
{
	m_stage_panel->GetDB()->Store(filename);
}

void Task::Clear()
{
	ee::ClearPanelSJ::Instance()->Clear();
}

void Task::InitLayout()
{
	wxSplitterWindow* right_split = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split);
	wxWindow* right = InitLayoutRight(right_split);

	left_split->SetSashGravity(0.3f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.75f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	m_main_panel = new MainPanel(parent);
	return m_main_panel;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage_panel = new StagePanel(parent, m_parent);
	return m_stage_panel;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_search_panel = new SearchPanel(split);
	m_warn_panel = new WarnPanel(split);

	split->SetSashGravity(0.5f);
	split->SplitHorizontally(m_search_panel, m_warn_panel);

	return split;
}

}
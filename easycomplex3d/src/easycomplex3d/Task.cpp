#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/LibraryPanel.h>
#include <ee/PropertySettingPanel.h>

#include <easysketch.h>

#include <wx/splitter.h>

namespace ecomplex3d
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

const ee::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::InitLayout()
{
	wxSplitterWindow* rightVerSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftVerSplitter = new wxSplitterWindow(rightVerSplitter);
	wxSplitterWindow* leftHorSplitter = new wxSplitterWindow(leftVerSplitter);

	m_library = new ee::LibraryPanel(leftHorSplitter);
	m_library->AddPage(new ecomplex3d::LibraryPage(m_library->GetNotebook()));

	m_property = new ee::PropertySettingPanel(leftHorSplitter);

	ecomplex3d::StagePanel* stage = new ecomplex3d::StagePanel(leftVerSplitter, m_parent, m_library);
	m_stage = stage;

	m_toolbar = new ecomplex3d::ToolbarPanel(rightVerSplitter, stage);

	leftHorSplitter->SetSashGravity(0.8f);
	leftHorSplitter->SplitHorizontally(m_library, m_property);
	leftVerSplitter->SetSashGravity(0.15f);
	leftVerSplitter->SplitVertically(leftHorSplitter, m_stage);
	
	rightVerSplitter->SetSashGravity(0.85f);
	rightVerSplitter->SplitVertically(leftVerSplitter, m_toolbar);

	m_root = rightVerSplitter;
}
	
}
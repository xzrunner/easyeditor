#include "Task.h"
#include "LibraryPanel.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"

namespace tdtex
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
	ee::BitmapMgr::Instance()->Clear();
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
	wxSplitterWindow* right_splitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);

	m_library = new LibraryPanel(left_splitter);
	m_stage = new StagePanel(left_splitter, m_parent, m_library);

	left_splitter->SetSashGravity(0.2f);
	left_splitter->SplitVertically(m_library, m_stage);

	ToolbarPanel* toolbar = new ToolbarPanel(right_splitter, static_cast<StagePanel*>(m_stage));
	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_splitter, toolbar);

	m_root = right_splitter;
}

}
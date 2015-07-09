#include "Task.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"

namespace eimage
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	initLayout();
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->Clear();
	d2d::BitmapMgr::Instance()->Clear();
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
	return m_stage->IsEditDirty();
}

void Task::Clear()
{
	m_library->Clear();
	m_stage->Clear();
}

const d2d::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::initLayout()
{
	wxSplitterWindow* rightVerSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftVerSplitter = new wxSplitterWindow(rightVerSplitter);
	wxSplitterWindow* leftHorSplitter = new wxSplitterWindow(leftVerSplitter);

	m_library = new d2d::LibraryPanel(leftHorSplitter);
	m_library->AddPage(new d2d::LibraryImagePage(m_library->GetNotebook()));

	m_property = new d2d::PropertySettingPanel(leftHorSplitter);

	m_stage = new StagePanel(leftVerSplitter, m_parent, m_library);
	m_property->SetEditPanel(m_stage);

	m_toolbar = new ToolbarPanel(rightVerSplitter, m_library, m_stage);

	leftHorSplitter->SetSashGravity(0.8f);
	leftHorSplitter->SplitHorizontally(m_library, m_property);
	leftVerSplitter->SetSashGravity(0.15f);
	leftVerSplitter->SplitVertically(leftHorSplitter, m_stage);
	
	rightVerSplitter->SetSashGravity(0.85f);
	rightVerSplitter->SplitVertically(leftVerSplitter, m_toolbar);

	m_root = rightVerSplitter;
}
	
}
#include "Task.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"

namespace esketch
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	initLayout();
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;
}

void Task::load(const char* filename)
{
}

void Task::store(const char* filename) const
{
}

bool Task::isDirty() const
{
	return m_stage->isDirty();
}

void Task::clear()
{
	m_library->clear();
	m_stage->clear();
	m_library->Refresh();
	m_stage->Refresh();
}

const d2d::EditPanel* Task::getEditPanel() const
{
	return m_stage;
}

void Task::initLayout()
{
	wxSplitterWindow* rightVerSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftVerSplitter = new wxSplitterWindow(rightVerSplitter);
	wxSplitterWindow* leftHorSplitter = new wxSplitterWindow(leftVerSplitter);

	m_library = new d2d::LibraryPanel(leftHorSplitter);
	m_library->addPage(new d2d::LibraryImagePage(m_library->getNotebook()));

	m_property = new d2d::PropertySettingPanel(leftHorSplitter);

	m_stage = new StagePanel(leftVerSplitter, m_parent, m_library);

	m_toolbar = new ToolbarPanel(rightVerSplitter, m_library, m_property, m_stage);

	leftHorSplitter->SetSashGravity(0.8f);
	leftHorSplitter->SplitHorizontally(m_library, m_property);
	leftVerSplitter->SetSashGravity(0.15f);
	leftVerSplitter->SplitVertically(leftHorSplitter, m_stage);
	
	rightVerSplitter->SetSashGravity(0.85f);
	rightVerSplitter->SplitVertically(leftVerSplitter, m_toolbar);

	m_root = rightVerSplitter;

	wxSize size = m_parent->GetSize();
	size.SetWidth(size.GetWidth() + 1);
	m_parent->SetSize(size);
}
	
}
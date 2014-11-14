#include "Task.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"
#include "StageCanvas.h"

#include <easycomplex.h>

namespace sg
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

void Task::load(const char* filepath)
{
	FileIO::load(filepath, m_stage, m_library);
}

void Task::store(const char* filepath) const
{
	FileIO::store(filepath, m_stage);
}

bool Task::isDirty() const
{
	return m_stage->isDirty();
}

void Task::clear()
{
	m_stage->clear();
	m_stage->Refresh();
}

const d2d::EditPanel* Task::getEditPanel() const
{
	return m_stage;
}

void Task::initLayout()
{
	wxSplitterWindow* rightSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftSplitter = new wxSplitterWindow(rightSplitter);
	
	m_library = new d2d::LibraryPanel(leftSplitter);	

	m_stage = new StagePanel(leftSplitter, m_parent, m_library);
	m_library->setCanvas(m_stage->getCanvas());

	ToolbarPanel* toolbar = new ToolbarPanel(rightSplitter, m_library, m_stage);

	leftSplitter->SetSashGravity(0.15f);
	leftSplitter->SplitVertically(m_library, m_stage);

	rightSplitter->SetSashGravity(0.9f);
	rightSplitter->SplitVertically(leftSplitter, toolbar);

	m_root = leftSplitter;
}

}
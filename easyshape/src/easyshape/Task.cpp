#include "Task.h"
#include "LibraryPanel.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"

namespace eshape
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_stage(NULL)
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
	FileIO::load(filename, m_library);
}

void Task::store(const char* filename) const
{
	FileIO::store(filename, m_library);
}

bool Task::isDirty() const
{
	return false;
}

void Task::clear()
{
	m_library->clear();
	m_stage->clear();
}

const d2d::EditPanel* Task::getEditPanel() const
{
	return m_stage;
}

void Task::initWindows(wxSplitterWindow* leftHorizontalSplitter, 
					   wxSplitterWindow* leftVerticalSplitter, 
					   wxSplitterWindow* rightVerticalSplitter, 
					   wxWindow*& library, wxWindow*& property, 
					   wxWindow*& stage, wxWindow*& toolbar)
{
	library = m_library = new LibraryPanel(leftHorizontalSplitter);

	property = m_property = new d2d::PropertySettingPanel(leftHorizontalSplitter);

	stage = m_stage = new StagePanel(leftVerticalSplitter, m_parent);

	toolbar = m_toolbar = new ToolbarPanel(rightVerticalSplitter, m_property, m_stage);

	m_library->setContext(m_stage, m_toolbar);
	m_stage->setToolbar(m_toolbar);
}

void Task::initLayout()
{
	wxSplitterWindow* rightVerticalSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftVerticalSplitter = new wxSplitterWindow(rightVerticalSplitter);
	wxSplitterWindow* leftHorizontalSplitter = new wxSplitterWindow(leftVerticalSplitter);

	wxWindow *library, *property, *stage, *toolbar;
	initWindows(leftHorizontalSplitter, leftVerticalSplitter, rightVerticalSplitter,
		library, property, stage, toolbar);

	if (library || property)
	{
		if (library && property)
		{
			leftHorizontalSplitter->SetSashGravity(0.8f);
			leftHorizontalSplitter->SplitHorizontally(library, property);
		}
		leftVerticalSplitter->SetSashGravity(0.15f);
		leftVerticalSplitter->SplitVertically(leftHorizontalSplitter, stage);
	}

	if (toolbar)
	{
		rightVerticalSplitter->SetSashGravity(0.85f);
		rightVerticalSplitter->SplitVertically(leftVerticalSplitter, toolbar);
	}

	m_root = rightVerticalSplitter;
}

}
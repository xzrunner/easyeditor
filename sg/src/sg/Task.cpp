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
	wxSplitterWindow* rightVerticalSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftVerticalSplitter = new wxSplitterWindow(rightVerticalSplitter);
	wxSplitterWindow* leftHorizontalSplitter = new wxSplitterWindow(leftVerticalSplitter);

	m_library = new d2d::LibraryPanel(leftHorizontalSplitter);	

	d2d::PropertySettingPanel* property = new d2d::PropertySettingPanel(leftHorizontalSplitter);

	m_stage = new StagePanel(leftVerticalSplitter, m_parent, m_library);
	m_library->setCanvas(m_stage->getCanvas());

	ToolbarPanel* toolbar = new ToolbarPanel(rightVerticalSplitter, m_library, property, m_stage);

	if (m_library || property)
	{
		if (m_library && property)
		{
			leftHorizontalSplitter->SetSashGravity(0.8f);
			leftHorizontalSplitter->SplitHorizontally(m_library, property);
		}
		leftVerticalSplitter->SetSashGravity(0.15f);
		leftVerticalSplitter->SplitVertically(leftHorizontalSplitter, m_stage);
	}

	if (toolbar)
	{
		rightVerticalSplitter->SetSashGravity(0.9f);
		rightVerticalSplitter->SplitVertically(leftVerticalSplitter, toolbar);
	}

	m_root = rightVerticalSplitter;

	wxSize size = m_parent->GetSize();
	size.SetWidth(size.GetWidth() + 1);
	m_parent->SetSize(size);
}

}
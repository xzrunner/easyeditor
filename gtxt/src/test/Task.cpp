#include "Task.h"
#include "LibraryPanel.h"
#include "StagePanel.h"

namespace tgtxt
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	InitLayout();
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
	return false;
}

const d2d::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::InitLayout()
{
	wxSplitterWindow* splitter = new wxSplitterWindow(m_parent);

 	m_library = new LibraryPanel(splitter);
 	m_stage = new StagePanel(splitter, m_parent, m_library);
 	m_library->SetCanvas(m_stage->GetCanvas());

 	splitter->SetSashGravity(0.2f);
 	splitter->SplitVertically(m_library, m_stage);
}

}
#include "Task.h"

#include <easyparticle3d.h>

namespace eparticle3d
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

	m_parent->SetTitle("EasyParticle");
}

void Task::Load(const char* filepath)
{
	if (!wxFileName::FileExists(filepath)) {
		throw d2d::Exception("File: %s don't exist!", filepath);
	}
	FileIO::Load(filepath, m_stage->m_ps, m_toolbar);
}

void Task::Store(const char* filepath) const
{
	FileIO::Store(filepath, m_toolbar);
}

bool Task::IsDirty() const
{
	return m_stage->IsEditDirty();
}

const d2d::EditPanel* Task::GetEditPanel() const 
{ 
	return m_stage; 
}

void Task::initLayout()
{
	wxSplitterWindow* rightSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftSplitter = new wxSplitterWindow(rightSplitter);

	m_library = new LibraryPanel(leftSplitter);
	m_stage = new StagePanel(leftSplitter, m_parent, m_library);
	m_toolbar = new ToolbarPanel(rightSplitter, m_library, m_stage);

	leftSplitter->SetSashGravity(0.2f);
	leftSplitter->SplitVertically(m_library, m_stage);

	rightSplitter->SetSashGravity(0.7f);
	rightSplitter->SplitVertically(leftSplitter, m_toolbar);

	m_root = rightSplitter;
}

void Task::StoreAsAnim(const wxString& filepath) const
{
	m_stage->m_ps->StoreAnimRecord(filepath.ToStdString());
}

void Task::StoreAsInvert(const wxString& filepath) const
{
	m_stage->m_ps->StoreInvertRecord(filepath.ToStdString());
}

}
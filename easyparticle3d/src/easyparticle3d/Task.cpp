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
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;

	m_parent->SetTitle("EasyParticle");
}

void Task::load(const char* filepath)
{
	if (!wxFileName::FileExists(filepath)) {
		throw d2d::Exception("File: %s don't exist!", filepath);
	}
	FileIO::load(filepath, m_stage->m_ps, m_toolbar);
}

void Task::store(const char* filepath) const
{
	FileIO::store(filepath, m_toolbar);
}

bool Task::isDirty() const
{
	return m_stage->isDirty();
}

void Task::clear()
{
	m_toolbar->initParticle();
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

	m_library = new LibraryPanel(leftSplitter);
	m_stage = new StagePanel(leftSplitter, m_parent, m_library);
	m_toolbar = new ToolbarPanel(rightSplitter, m_library, m_stage);

	m_library->SetCanvas(m_stage->getCanvas());

	leftSplitter->SetSashGravity(0.2f);
	leftSplitter->SplitVertically(m_library, m_stage);

	rightSplitter->SetSashGravity(0.7f);
	rightSplitter->SplitVertically(leftSplitter, m_toolbar);

	m_root = rightSplitter;
}

void Task::StoreAsAnim(const wxString& filepath) const
{
	m_stage->m_ps->StoreRecordAsAnimFile(filepath.ToStdString());
}

}
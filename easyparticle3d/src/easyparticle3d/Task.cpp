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

void Task::loadFromFile(const char* filename)
{
	if (!wxFileName::FileExists(filename)) {
		throw d2d::Exception("File: %s don't exist!", filename);
	}
	FileIO::load(filename, m_stage->m_ps, m_toolbar);
}

void Task::storeToFile(const char* filename) const
{
	FileIO::store(filename, m_toolbar);
}

void Task::clear()
{
	m_toolbar->initParticle();
	m_stage->clear();
	m_stage->Refresh();
}

d2d::GLCanvas* Task::getCanvas() const
{
	return m_stage->getCanvas();
}

void Task::initLayout()
{
	wxSplitterWindow* rightSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftSplitter = new wxSplitterWindow(rightSplitter);

	m_library = new LibraryPanel(leftSplitter);
	m_stage = new StagePanel(leftSplitter, m_parent, m_library);
	m_toolbar = new ToolbarPanel(rightSplitter, m_library, m_stage);

	m_library->setCanvas(m_stage->getCanvas());

	leftSplitter->SetSashGravity(0.2f);
	leftSplitter->SplitVertically(m_library, m_stage);

	rightSplitter->SetSashGravity(0.7f);
	rightSplitter->SplitVertically(leftSplitter, m_toolbar);

	m_root = rightSplitter;
}

}
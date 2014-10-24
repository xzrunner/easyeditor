#include "Task.h"

#include <easyparticle2d.h>

namespace eparticle2d
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
}

void Task::storeToFile(const char* filename) const
{
}

void Task::clear()
{
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
	m_toolbar = new ToolbarPanel(rightSplitter, m_library, m_stage, 
		m_stage->GetParticleSystem());

	leftSplitter->SetSashGravity(0.2f);
	leftSplitter->SplitVertically(m_library, m_stage);

	rightSplitter->SetSashGravity(0.7f);
	rightSplitter->SplitVertically(leftSplitter, m_toolbar);

	m_root = rightSplitter;

// 	wxSize size = m_parent->GetSize();
// 	size.SetWidth(size.GetWidth() + 1);
// 	m_parent->SetSize(size);
}

}
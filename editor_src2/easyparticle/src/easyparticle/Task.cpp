#include "Task.h"
#include "coco/StagePanel.h"
#include "coco/ToolbarPanel.h"
#include "LibraryPanel.h"
#include "Context.h"
#include "FileIO.h"

using namespace eparticle;

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

void Task::loadFromTextFile(const char* filename)
{
	FileIO::load(filename);
}

void Task::storeToTextFile(const char* filename) const
{
	FileIO::store(filename);
}

void Task::clear()
{
	Context::Instance()->stage->clear();
	Context::Instance()->stage->Refresh();
}

d2d::GLCanvas* Task::getCanvas() const
{
	if (Context::Instance()->stage)
		return Context::Instance()->stage->getCanvas();
	else
		return NULL;
}

void Task::initWindows(wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter, 
					   wxWindow*& library, wxWindow*& stage, wxWindow*& toolbar)
{
	library = Context::Instance()->library = new eparticle::LibraryPanel(leftVerticalSplitter);
	stage = Context::Instance()->stage = new coco::StagePanel(leftVerticalSplitter, m_parent);
	toolbar = Context::Instance()->toolbar = new coco::ToolbarPanel(rightVerticalSplitter, Context::Instance()->library);
}

void Task::initLayout()
{
	wxSplitterWindow* rightVerticalSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftVerticalSplitter = new wxSplitterWindow(rightVerticalSplitter);

	wxWindow *library, *stage, *toolbar;
	initWindows(leftVerticalSplitter, rightVerticalSplitter, library, stage, toolbar);

	leftVerticalSplitter->SetSashGravity(0.2f);
	leftVerticalSplitter->SplitVertically(library, stage);

	rightVerticalSplitter->SetSashGravity(0.7f);
	rightVerticalSplitter->SplitVertically(leftVerticalSplitter, toolbar);

	m_root = rightVerticalSplitter;

	wxSize size = m_parent->GetSize();
	size.SetWidth(size.GetWidth() + 1);
	m_parent->SetSize(size);
}
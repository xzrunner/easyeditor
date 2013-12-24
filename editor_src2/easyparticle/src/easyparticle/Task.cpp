
#include "Task.h"
#include "coco/StagePanel.h"
#include "coco/ToolbarPanel.h"
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

void Task::initWindows(wxSplitterWindow* splitter, 
					   wxWindow*& stage, wxWindow*& toolbar)
{
	stage = Context::Instance()->stage = new coco::StagePanel(splitter, m_parent);
	toolbar = Context::Instance()->toolbar = new coco::ToolbarPanel(splitter);
}

void Task::initLayout()
{
	wxSplitterWindow* splitter = new wxSplitterWindow(m_parent);

	wxWindow *stage, *toolbar;
	initWindows(splitter, stage, toolbar);

	splitter->SetSashGravity(0.7f);
	splitter->SplitVertically(stage, toolbar);

	m_root = splitter;

	wxSize size = m_parent->GetSize();
	size.SetWidth(size.GetWidth() + 1);
	m_parent->SetSize(size);
}
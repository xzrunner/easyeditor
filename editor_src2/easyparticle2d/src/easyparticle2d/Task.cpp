#include "Task.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "Context.h"
#include "FileIO.h"

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
//	FileIO::load(filename);
}

void Task::storeToFile(const char* filename) const
{
//	FileIO::store(filename);
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

void Task::initLayout()
{
	wxSplitterWindow* split = new wxSplitterWindow(m_parent);
	Context* context = Context::Instance();
	context->stage = new StagePanel(split, m_parent);
	context->toolbar = new ToolbarPanel(split);
	split->SetSashGravity(0.8f);
	split->SplitVertically(context->stage, context->toolbar);

	m_root = split;
}

}
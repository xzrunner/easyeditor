#include "Task.h"
#include "StagePanel.h"
#include "FileIO.h"
#include "Context.h"
#include "TreeCtrl.h"

using namespace edb;

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

void Task::loadFromFile(const char* filename)
{
	FileIO::load(filename);
}

void Task::storeToFile(const char* filename) const
{
	FileIO::store(filename);
}

void Task::clear()
{
	Context* context = Context::Instance();

	context->stage->clear();
	context->stage->Refresh();
}

void Task::initLayout()
{
	wxSplitterWindow* splitter = new wxSplitterWindow(m_parent);

	Context* context = Context::Instance();

	StagePanel* stage = context->stage = new StagePanel(splitter, m_parent);
	TreeCtrl* tree = context->tree = new TreeCtrl(splitter);

	splitter->SetSashGravity(0.2f);
	splitter->SplitVertically(tree, stage);
}
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
	d2d::SymbolMgr::Instance()->Clear();
	d2d::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::loadFromFile(const char* filename)
{
	if (!wxFileName::FileExists(filename)) {
		throw d2d::Exception("File: %s don't exist!", filename);
	}

	FileIO::load(filename);
}

void Task::storeToFile(const char* filename) const
{
	FileIO::store(filename);
}

void Task::clear()
{
	d2d::ClearPanelSJ::Instance()->Clear();
}

void Task::initLayout()
{
	wxSplitterWindow* splitter = new wxSplitterWindow(m_parent);

	Context* context = Context::Instance();

	StagePanel* stage = context->stage = new StagePanel(splitter, m_parent, NULL);
	TreeCtrl* tree = context->tree = new TreeCtrl(splitter);

	splitter->SetSashGravity(0.2f);
	splitter->SplitVertically(tree, stage);
}
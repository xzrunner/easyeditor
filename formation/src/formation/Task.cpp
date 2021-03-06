#include "Task.h"
#include "StagePanel.h"
#include "LibraryImagePage.h"
#include "ToolbarPanel.h"
#include "FileIO.h"
#include "Context.h"

using namespace formation;

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	initLayout();
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->clear();
	ee::BitmapMgr::Instance()->clear();
	delete m_root;
}

void Task::loadFromFile(const char* filename)
{
	if (!wxFileName::FileExists(filename)) {
		throw ee::Exception("File: %s don't exist!", filename);
	}
	FileIO::load(filename);
}

void Task::storeToFile(const char* filename) const
{
	FileIO::store(filename);
}

void Task::clear()
{
	Context* context = Context::Instance();

	context->library->clear();
	context->stage->clear();
	context->library->Refresh();
	context->stage->Refresh();
}

void Task::initWindows(wxSplitterWindow* leftHorizontalSplitter, 
					   wxSplitterWindow* leftVerticalSplitter, 
					   wxSplitterWindow* rightVerticalSplitter, 
					   wxWindow*& library, wxWindow*& property, 
					   wxWindow*& stage, wxWindow*& toolbar)
{
	Context* context = Context::Instance();

	library = context->library = new ee::LibraryPanel(leftHorizontalSplitter);
	context->library->addPage(new LibraryImagePage(context->library->getNotebook()));

	property = context->property = new ee::PropertySettingPanel(leftHorizontalSplitter);

	stage = context->stage = new StagePanel(leftVerticalSplitter, m_parent);

	toolbar = context->toolbar = new ToolbarPanel(rightVerticalSplitter);
}

void Task::initLayout()
{
	wxSplitterWindow* rightVerticalSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftVerticalSplitter = new wxSplitterWindow(rightVerticalSplitter);
	wxSplitterWindow* leftHorizontalSplitter = new wxSplitterWindow(leftVerticalSplitter);

	wxWindow *library, *property, *stage, *toolbar;
	initWindows(leftHorizontalSplitter, leftVerticalSplitter, rightVerticalSplitter,
		library, property, stage, toolbar);

	if (library || property)
	{
		if (library && property)
		{
			leftHorizontalSplitter->SetSashGravity(0.8f);
			leftHorizontalSplitter->SplitHorizontally(library, property);
		}
		leftVerticalSplitter->SetSashGravity(0.15f);
		leftVerticalSplitter->SplitVertically(leftHorizontalSplitter, stage);
	}

	if (toolbar)
	{
		rightVerticalSplitter->SetSashGravity(0.85f);
		rightVerticalSplitter->SplitVertically(leftVerticalSplitter, toolbar);
	}

	m_root = rightVerticalSplitter;
}
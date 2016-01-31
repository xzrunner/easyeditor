#include "Task.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"
#include "Context.h"

using namespace etexpacker;

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	InitLayout();
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	ee::BitmapMgr::Instance()->Clear();
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
	ee::ClearPanelSJ::Instance()->Clear();
}

void Task::initWindows(wxSplitterWindow* leftHorizontalSplitter, 
					   wxSplitterWindow* leftVerticalSplitter, 
					   wxSplitterWindow* rightVerticalSplitter, 
					   wxWindow*& library, wxWindow*& property, 
					   wxWindow*& stage, wxWindow*& toolbar)
{
	Context* context = Context::Instance();

	library = context->library = new ee::LibraryPanel(leftHorizontalSplitter);
	context->library->AddPage(new ee::LibraryImagePage(context->library->GetNotebook()));

	property = context->property = new ee::PropertySettingPanel(leftHorizontalSplitter);

	stage = context->stage = new StagePanel(leftVerticalSplitter, m_parent);
	context->property->SetEditPanel(context->stage->GetStageImpl());

	toolbar = context->toolbar = new ToolbarPanel(rightVerticalSplitter, context->stage);
}

void Task::InitLayout()
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
		rightVerticalSplitter->SetSashGravity(0.83f);
		rightVerticalSplitter->SplitVertically(leftVerticalSplitter, toolbar);
	}

	m_root = rightVerticalSplitter;
}
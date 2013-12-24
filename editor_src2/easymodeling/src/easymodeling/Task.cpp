
#include "Task.h"
#include "LibraryPanel.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "PreviewDialog.h"
#include "FileIO.h"
#include "Context.h"
#include "WorldPropertySetting.h"

using namespace emodeling;

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

	m_parent->SetTitle("EasyMoodeling");
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
	Context* context = Context::Instance();
	context->library->clear();
	context->stage->clear();
	context->library->Refresh();
	context->stage->Refresh();
}

void Task::onPreview() const
{
	PreviewDialog dlg;
	dlg.ShowModal();
	Context::Instance()->stage->resetCanvas();
}

d2d::GLCanvas* Task::getCanvas() const
{
	if (d2d::EditPanel* stage = Context::Instance()->stage)
		return stage->getCanvas();
	else
		return NULL;
}

void Task::initWindows(wxSplitterWindow* leftHorizontalSplitter, 
					   wxSplitterWindow* leftVerticalSplitter, 
					   wxSplitterWindow* rightVerticalSplitter, 
					   wxWindow*& library, wxWindow*& property, 
					   wxWindow*& stage, wxWindow*& toolbar)
{
	Context* context = Context::Instance();

	library = context->library = new LibraryPanel(leftHorizontalSplitter);

	property = context->property = new d2d::PropertySettingPanel(leftHorizontalSplitter);

	stage = context->stage = new StagePanel(leftVerticalSplitter, m_parent);

	toolbar = context->toolbar = new ToolbarPanel(rightVerticalSplitter);

	context->property->setPropertySetting(new WorldPropertySetting(context->stage));
}

void Task::initLayout()
{
	wxSplitterWindow* rightVerticalSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftVerticalSplitter = new wxSplitterWindow(rightVerticalSplitter);
	wxSplitterWindow* leftHorizontalSplitter = new wxSplitterWindow(leftVerticalSplitter);

	wxWindow *library, *property, *stage, *toolbar;
	initWindows(leftHorizontalSplitter, leftVerticalSplitter, rightVerticalSplitter,
		library, property, stage, toolbar);

	assert(stage);

	if (library || property)
	{
		if (library && property)
		{
			leftHorizontalSplitter->SetSashGravity(0.7f);
			leftHorizontalSplitter->SplitHorizontally(library, property);
		}
		leftVerticalSplitter->SetSashGravity(0.15f);
		leftVerticalSplitter->SplitVertically(leftHorizontalSplitter, stage);
	}

	if (toolbar)
	{
		rightVerticalSplitter->SetSashGravity(0.9f);
		rightVerticalSplitter->SplitVertically(leftVerticalSplitter, toolbar);
	}

	m_root = rightVerticalSplitter;

	wxSize size = m_parent->GetSize();
	size.SetWidth(size.GetWidth() + 1);
	m_parent->SetSize(size);
}
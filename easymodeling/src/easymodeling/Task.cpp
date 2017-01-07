#include "Task.h"
#include "LibraryPanel.h"
#include "PropertySettingPanel.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "PreviewDialog.h"
#include "FileIO.h"
#include "Context.h"
#include "WorldPropertySetting.h"

#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/panel_msg.h>

namespace emodeling
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	InitLayout();
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	delete m_root;

	m_parent->SetTitle("EasyMoodeling");
}

void Task::LoadFromFile(const char* filename)
{
	if (!wxFileName::FileExists(filename)) {
		throw ee::Exception("File: %s don't exist!", filename);
	}
  	FileIO::Load(filename);
}

void Task::StoreToFile(const char* filename) const
{
  	FileIO::Store(filename);
}

void Task::Clear()
{
	ee::ClearPanelSJ::Instance()->Clear();
}

void Task::OnPreview() const
{
	PreviewDialog dlg;
	dlg.ShowModal();
}

ee::StageCanvas* Task::GetCanvas() const
{
	if (ee::EditPanel* stage = Context::Instance()->stage)
		return stage->GetCanvas();
	else
		return NULL;
}

void Task::InitWindows(wxSplitterWindow* leftHorizontalSplitter, 
					   wxSplitterWindow* leftVerticalSplitter, 
					   wxSplitterWindow* rightVerticalSplitter, 
					   wxWindow*& library, wxWindow*& property, 
					   wxWindow*& stage, wxWindow*& toolbar)
{
	Context* context = Context::Instance();

	library = context->library = new LibraryPanel(leftHorizontalSplitter);

	property = context->property = new PropertySettingPanel(leftHorizontalSplitter);

	stage = context->stage = new StagePanel(leftVerticalSplitter, m_parent);
	context->property->SetEditPanel(context->stage->GetStageImpl());

	toolbar = context->toolbar = new ToolbarPanel(rightVerticalSplitter);

	context->property->SetPropertySetting(new WorldPropertySetting(context->stage->GetStageImpl()));
}

void Task::InitLayout()
{
	wxSplitterWindow* rightVerticalSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftVerticalSplitter = new wxSplitterWindow(rightVerticalSplitter);
	wxSplitterWindow* leftHorizontalSplitter = new wxSplitterWindow(leftVerticalSplitter);

	wxWindow *library, *property, *stage, *toolbar;
	InitWindows(leftHorizontalSplitter, leftVerticalSplitter, rightVerticalSplitter,
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
}

}
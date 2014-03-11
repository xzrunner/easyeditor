#include "Task.h"
#include "FileIO.h"

#include <easycomplex.h>

namespace ecomplex
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_library(NULL)
	, m_property(NULL)
	, m_stage(NULL)
	, m_viewlist(NULL)
{
	initLayout();
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;
}

void Task::load(const char* filepath)
{
	FileIO::load(this, filepath);
}

void Task::store(const char* filepath) const
{
	FileIO::store(this, filepath);
	m_stage->onSave();
}

bool Task::isDirty() const
{
	return m_stage->isDirty();
}

void Task::clear()
{
	//	d2d::SymbolMgr::Instance()->clear();

	m_viewlist->clear();

	m_library->clear();
	m_stage->clear();
	m_library->Refresh();
	m_stage->Refresh();
}

void Task::getAllSprite(std::vector<const d2d::ISprite*>& sprites) const
{
	m_stage->traverseSprites(d2d::FetchAllVisitor<const d2d::ISprite>(sprites));
}

const d2d::EditPanel* Task::getEditPanel() const
{
	return m_stage;
}

void Task::initWindows(wxSplitterWindow* leftHorizontalSplitter, 
	wxSplitterWindow* leftVerticalSplitter, 
	wxSplitterWindow* rightVerticalSplitter, 
	wxWindow*& library, wxWindow*& property, 
	wxWindow*& stage, wxWindow*& toolbar)
{
	library = m_library = new ecomplex::LibraryPanel(leftHorizontalSplitter);

	property = m_property = new d2d::PropertySettingPanel(leftHorizontalSplitter);

	stage = m_stage = new ecomplex::StagePanel(leftVerticalSplitter, m_parent, m_property, m_library);
	m_property->setPropertySetting(new ecomplex::PropertySetting(m_stage, m_stage->getSymbol()));

//		context->toolbar = new ToolbarPanel(rightVerticalSplitter, context->stage, context->property);

	toolbar = m_viewlist = new d2d::ViewlistPanel(rightVerticalSplitter, m_stage, m_stage, m_property);

	m_stage->setViewlist(m_viewlist);
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
			leftHorizontalSplitter->SetSashGravity(0.5f);
			leftHorizontalSplitter->SplitHorizontally(library, property);
		}
		leftVerticalSplitter->SetSashGravity(0.2f);
		leftVerticalSplitter->SplitVertically(leftHorizontalSplitter, stage);
	}

	if (toolbar)
	{
		rightVerticalSplitter->SetSashGravity(0.85f);
		rightVerticalSplitter->SplitVertically(leftVerticalSplitter, toolbar);
	}

	m_root = rightVerticalSplitter;

	wxSize size = m_parent->GetSize();
	size.SetWidth(size.GetWidth() + 1);
	m_parent->SetSize(size);
}

}
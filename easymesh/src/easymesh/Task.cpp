#include "Task.h"

#include <easymesh.h>

#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
#include <ee/FileType.h>
#include <ee/LibraryPanel.h>
#include <ee/LibraryImagePage.h>
#include <ee/PropertySettingPanel.h>

namespace emesh
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_library(NULL)
	, m_property(NULL)
	, m_stage(NULL)
{
	InitLayout();
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	ee::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filepath)
{
	if (ee::FileType::IsType(filepath, ee::FileType::e_mesh)) {
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		Symbol* msymbol = static_cast<Symbol*>(symbol);
		m_stage->SetSymbol(msymbol);
		
		m_library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
		symbol->Release();
	}
}

void Task::Store(const char* filepath) const
{
	FileIO::Store(filepath, m_stage->GetSymbol());
	m_stage->OnSave();
}

bool Task::IsDirty() const
{
	return false;
}

const ee::EditPanel* Task::GetEditPanel() const 
{ 
	return m_stage; 
}

void Task::initWindows(wxSplitterWindow* leftHorizontalSplitter, 
	wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
	wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar)
{
	library = m_library = new ee::LibraryPanel(leftHorizontalSplitter);
	m_library->AddPage(new ee::LibraryImagePage(m_library->GetNotebook()));

	property = m_property = new ee::PropertySettingPanel(leftHorizontalSplitter);

	stage = m_stage = new emesh::StagePanel(leftVerticalSplitter, m_parent, NULL, m_library);
//	m_property->SetPropertySetting(new complex::PropertySetting(m_stage, m_sstage->getSymbol()));
	m_property->SetEditPanel(m_stage->GetStageImpl());

	toolbar = new emesh::ToolbarPanel(rightVerticalSplitter, m_stage, true, NULL);
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
}

}
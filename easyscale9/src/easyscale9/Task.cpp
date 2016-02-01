#include "Task.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"

#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
#include <ee/FileType.h>
#include <ee/LibraryPanel.h>
#include <ee/LibraryImagePage.h>
#include <ee/PropertySettingPanel.h>

namespace escale9
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
	ee::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filepath)
{
	if (ee::FileType::IsType(filepath, ee::FileType::e_scale9)) {
		FileIO::Load(filepath, m_library, m_stage, m_toolbar);
	}
}

void Task::Store(const char* filepath) const
{
	FileIO::Store(filepath, m_stage, m_toolbar);
}

bool Task::IsDirty() const
{
	return m_stage->IsEditDirty();
}

const ee::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::InitLayout()
{
	wxSplitterWindow* right_split = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split);
	wxWindow* right = InitLayoutRight(right_split);

	left_split->SetSashGravity(0.2f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_library = new ee::LibraryPanel(split);
	m_library->AddPage(new ee::LibraryImagePage(m_library->GetNotebook()));

	m_property = new ee::PropertySettingPanel(split);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new StagePanel(parent, m_parent, m_library);
	m_property->SetEditPanel(m_stage->GetStageImpl());

	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	m_toolbar = new ToolbarPanel(parent, m_stage, m_property);
	m_stage->setToolbarPanel(m_toolbar);
	return m_toolbar;
}

} // escale9
#include "Task.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"

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
	d2d::SymbolMgr::Instance()->Clear();
	d2d::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filepath)
{
	if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_scale9)) {
		FileIO::load(filepath, m_library, m_stage, m_toolbar);
		m_stage->GetCanvas()->ResetViewport();
	}
}

void Task::Store(const char* filepath) const
{
	FileIO::store(filepath, m_stage, m_toolbar);
}

bool Task::IsDirty() const
{
	return m_stage->IsEditDirty();
}

void Task::Clear()
{
	m_library->Clear();
	m_stage->Clear();

}

const d2d::EditPanel* Task::GetEditPanel() const
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

	m_library = new d2d::LibraryPanel(split);
	m_library->AddPage(new d2d::LibraryImagePage(m_library->GetNotebook()));

	m_property = new d2d::PropertySettingPanel(split);
	m_view_panel_mgr.AddSpritePanel(m_property);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new StagePanel(parent, m_parent, m_library);
	m_library->SetCanvas(m_stage->GetCanvas());
	m_property->SetEditPanel(m_stage->GetStageImpl());
	m_view_panel_mgr.AddSpritePanel(m_stage);

	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	m_toolbar = new ToolbarPanel(parent, m_stage, m_property, &m_view_panel_mgr);
	m_stage->setToolbarPanel(m_toolbar);
	return m_toolbar;
}

} // escale9
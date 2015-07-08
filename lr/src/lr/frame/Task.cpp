#include "Task.h"
#include "FileIO.h"
#include "ToolBar.h"
#include "Frame.h"

#include "view/LibraryPanel.h"
#include "view/StagePanel.h"

namespace lr
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	InitLayout();
	m_stage->GetBaseOP()->OnActive();
	static_cast<Frame*>(parent)->GetToolBar()->SetLibrary(m_library);
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->Clear();
	d2d::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filename)
{
	FileIO::Load(filename, m_library, m_stage, m_grouptree);
	m_stage->GetCanvas()->ResetViewport();
}

void Task::Store(const char* filename) const
{
	FileIO::Store(filename, m_library, m_stage, m_grouptree);
}

bool Task::IsDirty() const
{
	return true;
}

void Task::Clear()
{
	m_viewlist->Clear();

	m_library->Clear();
	m_stage->Clear();
	m_library->Refresh();
	m_stage->RefreshStage();
}

void Task::GetAllSprite(std::vector<const d2d::ISprite*>& sprites) const
{
	m_stage->TraverseSprites(d2d::FetchAllVisitor<const d2d::ISprite>(sprites));
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

	m_library = new LibraryPanel(split);

	m_property = new d2d::PropertySettingPanel(split);
	m_view_panel_mgr.AddSpritePanel(m_property);
	m_view_panel_mgr.AddShapePanel(m_property);

	split->SetSashGravity(0.75f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new StagePanel(parent, m_parent, m_property, m_library, &m_view_panel_mgr);
	m_view_panel_mgr.AddSpritePanel(m_stage);
	m_view_panel_mgr.AddShapePanel(m_stage);

	m_library->SetStagePanel(m_stage);
	m_library->InitPages(m_stage, m_property, &m_view_panel_mgr);
	m_library->SetCanvas(m_stage->GetCanvas());
	m_property->SetEditPanel(m_stage);

	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_viewlist = new d2d::ViewlistPanel(split, m_stage, m_stage, &m_view_panel_mgr);
	m_library->SetViewlist(m_viewlist);
	m_view_panel_mgr.AddSpritePanel(m_viewlist);

	m_grouptree = new d2d::GroupTreePanel(split, m_stage, &m_view_panel_mgr);
	m_library->SetGroupTree(m_grouptree);
	m_view_panel_mgr.AddSpritePanel(m_grouptree);

	split->SetSashGravity(0.5f);
	split->SplitHorizontally(m_viewlist, m_grouptree);

	return split;
}

}
#include "Task.h"
#include "FileIO.h"

#include "view/StagePanel.h"
#include "view/LibraryPanel.h"
#include "view/ToolbarPanel.h"

namespace eui
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

void Task::Load(const char* filename)
{
//	FileIO::Load(filename, m_stage, m_library);
}

void Task::Store(const char* filename) const
{
//	FileIO::Store(filename, m_stage);
}

bool Task::IsDirty() const
{
	return false;
}

void Task::Clear()
{
	m_top_pannels.Clear();
}

void Task::GetAllSprite(std::vector<const d2d::ISprite*>& sprites) const
{
//	m_stage->TraverseSprites(d2d::FetchAllVisitor<const d2d::ISprite>(sprites));
}

const d2d::EditPanel* Task::GetEditPanel() const
{
//	return m_stage;
	return NULL;
}

void Task::InitLayout()
{
	wxSplitterWindow* right_split = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* right = InitLayoutRight(right_split);
	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split);

	left_split->SetSashGravity(0.12f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_top_pannels.library = new LibraryPanel(split);
	m_top_pannels.property = new d2d::PropertySettingPanel(split);
	m_top_pannels.view_panel_mgr.AddSpritePanel(m_top_pannels.property);

	split->SetSashGravity(0.6f);
	split->SplitHorizontally(m_top_pannels.library, m_top_pannels.property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_top_pannels.stage = new StagePanel(parent, m_parent, &m_top_pannels);
//	m_view_panel_mgr.AddSpritePanel(m_stage);

//	m_library->SetCanvas(m_stage->GetCanvas());
//	m_property->SetEditPanel(m_stage->GetStageImpl());

	return m_top_pannels.stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
//	wxSplitterWindow* split = new wxSplitterWindow(parent);

// 	d2d::ViewlistPanel* viewlist = new d2d::ViewlistPanel(split, m_stage->GetStageImpl(), m_stage, &m_view_panel_mgr);
// 	m_view_panel_mgr.AddSpritePanel(viewlist);
// 	
// 	d2d::GroupTreePanel* grouptree = new d2d::GroupTreePanel(split, m_stage, &m_view_panel_mgr, m_stage->GetKeyState());
// 	m_view_panel_mgr.AddSpritePanel(grouptree);
// 
// 	split->SetSashGravity(0.5f);
// 	split->SplitHorizontally(viewlist, grouptree);

//	return split;

	m_top_pannels.toolbar = new ToolbarPanel(parent);
	return m_top_pannels.toolbar;
}

}
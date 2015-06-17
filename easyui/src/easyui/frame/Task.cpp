#include "Task.h"
#include "FileIO.h"

#include "view/StagePanel.h"

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
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;
}

void Task::load(const char* filename)
{
	FileIO::Load(filename, m_stage, m_library);
}

void Task::store(const char* filename) const
{
	FileIO::Store(filename, m_stage);
}

bool Task::isDirty() const
{
	return false;
}

void Task::clear()
{
	m_library->Clear();
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

AnchorMgr* Task::GetAnchorMgr()
{
	return m_stage->GetAnchorMgr();
}

void Task::InitLayout()
{
	wxSplitterWindow* right_split = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split);
	wxWindow* right = InitLayoutRight(right_split);

	left_split->SetSashGravity(0.12f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_library = new d2d::LibraryPanel(split);

	m_property = new d2d::PropertySettingPanel(split);

	split->SetSashGravity(0.6f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new StagePanel(parent, m_parent, m_property, m_library, &m_view_panel_mgr);

	m_view_panel_mgr.AddPanel(m_stage);
	m_library->SetCanvas(m_stage->getCanvas());

	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	d2d::ViewlistPanel* viewlist = new d2d::ViewlistPanel(split, m_stage, m_stage, m_property, &m_view_panel_mgr);
	m_view_panel_mgr.AddPanel(viewlist);
	
	d2d::GroupTreePanel* grouptree = new d2d::GroupTreePanel(split, m_stage, &m_view_panel_mgr);
	m_view_panel_mgr.AddPanel(grouptree);

	split->SetSashGravity(0.5f);
	split->SplitHorizontally(viewlist, grouptree);

	return split;
}

}
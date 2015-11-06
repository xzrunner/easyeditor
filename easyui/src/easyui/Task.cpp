#include "Task.h"
#include "FileIO.h"
#include "UIStagePage.h"
#include "TopStagePanel.h"
#include "TopLibraryPanel.h"
#include "TopToolbarPanel.h"

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
	UIStagePage* stage = m_top_pannels.stage->GetSelectedPage();
	if (stage) {
		stage->LoadFromFile(filename);
	}
}

void Task::Store(const char* filename) const
{
	UIStagePage* stage = m_top_pannels.stage->GetSelectedPage();
	if (stage) {
		stage->StoreToFile(filename);
	}
}

UIStagePage* Task::GetSelectedStagePage()
{
	return m_top_pannels.stage->GetSelectedPage();
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

	m_top_pannels.library = new TopLibraryPanel(split);
	m_top_pannels.property = new d2d::PropertySettingPanel(split);

	split->SetSashGravity(0.6f);
	split->SplitHorizontally(m_top_pannels.library, m_top_pannels.property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_top_pannels.stage = new TopStagePanel(parent, m_parent, &m_top_pannels);
	return m_top_pannels.stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	m_top_pannels.toolbar = new TopToolbarPanel(parent);
	return m_top_pannels.toolbar;
}

}
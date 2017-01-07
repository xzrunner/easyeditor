#include "Task.h"
#include "FileLoader.h"
#include "FileStorer.h"
#include "StagePanel.h"
#include "LibraryPanel.h"

#include <ee/SymbolMgr.h>
#include <ee/PropertySettingPanel.h>
#include <ee/ViewlistPanel.h>
#include <ee/GroupTreePanel.h>
#include <ee/FetchAllVisitor.h>
#include <ee/sprite_msg.h>

#include <algorithm>

namespace eskeleton
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
	delete m_root;
}

void Task::Load(const char* filename)
{
	std::vector<ee::Sprite*> sprs;
	FileLoader::Load(filename, sprs);
	for (int i = 0, n = sprs.size(); i < sprs.size(); ++i) {
		ee::InsertSpriteSJ::Instance()->Insert(sprs[i]);
	}
	for_each(sprs.begin(), sprs.end(), cu::RemoveRefFunctor<ee::Sprite>());
}

void Task::Store(const char* filename) const
{
	std::vector<ee::Sprite*> sprs;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	FileStorer::Store(filename, sprs);
}

bool Task::IsDirty() const
{
	return false;
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

	m_library = new LibraryPanel(split);

	m_property = new ee::PropertySettingPanel(split);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	StagePanel* stage = new StagePanel(parent, m_parent, m_library, m_property);
	m_stage = stage;
	m_property->SetEditPanel(m_stage->GetStageImpl());
	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_viewlist = new ee::ViewlistPanel(split);

	m_grouptree = new ee::GroupTreePanel(split, m_stage, m_stage->GetKeyState());

	split->SetSashGravity(0.5f);
	split->SplitHorizontally(m_viewlist, m_grouptree);

	return split;
}

}
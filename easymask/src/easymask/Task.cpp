#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
#include <ee/PropertySettingPanel.h>
#include <ee/FileType.h>
#include <ee/LibraryPage.h>
#include <ee/LibraryList.h>

#include <easymask.h>

namespace emask
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
	if (!ee::FileType::IsType(filepath, ee::FileType::e_mask)) {
		return;
	}

	FileIO::Load(filepath, m_stage->GetSymbol());

	Symbol* sym = m_stage->GetSymbol();
	if (ee::Sprite* spr = sym->GetSprite(true)) {
		ee::LibraryList* base_list = m_library->GetPage(true)->GetList();
		ee::Symbol* base_sym = const_cast<ee::Symbol*>(&spr->GetSymbol());
		base_list->Insert(base_sym);
	}
	if (ee::Sprite* spr = sym->GetSprite(false)) {
		ee::LibraryList* mask_list = m_library->GetPage(false)->GetList();
		ee::Symbol* mask_sym = const_cast<ee::Symbol*>(&spr->GetSymbol());
		mask_list->Insert(mask_sym);
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
	m_stage = new StagePanel(parent, m_parent, m_property, m_library);
	m_property->SetEditPanel(m_stage->GetStageImpl());
	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	m_toolbar = new ToolbarPanel(parent, m_stage);
	return m_toolbar;
}

}
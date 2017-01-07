#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/SymbolFile.h>
#include <ee/LibraryPage.h>
#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>

#include <easymask.h>

#include <sprite2/SymType.h>

namespace emask
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_library(NULL)
	, m_stage(NULL)
{
	InitLayout();
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filepath)
{
	if (ee::SymbolFile::Instance()->Type(filepath) != s2::SYM_MASK) {
		return;
	}

	FileIO::Load(filepath, m_stage->GetSymbol());

	Symbol* sym = m_stage->GetSymbol();
	if (const s2::Sprite* base = sym->GetBase()) {
		ee::LibraryList* base_list = m_library->GetPage(true)->GetList();
		base_list->Insert(const_cast<ee::Symbol*>(dynamic_cast<const ee::Symbol*>(base->GetSymbol())));
	}
	if (const s2::Sprite* mask = sym->GetMask()) {
		ee::LibraryList* mask_list = m_library->GetPage(false)->GetList();
		mask_list->Insert(const_cast<ee::Symbol*>(dynamic_cast<const ee::Symbol*>(mask->GetSymbol())));
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

	m_library = new LibraryPanel(left_split);
	m_stage = new StagePanel(left_split, m_parent, m_library);
	m_toolbar = new ToolbarPanel(right_split, m_stage);

	left_split->SetSashGravity(0.2f);
	left_split->SplitVertically(m_library, m_stage);

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, m_toolbar);

	m_root = right_split;
}

}
#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
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
	if (const s2::Symbol* base = sym->GetSymbol(true)) {
		ee::LibraryList* base_list = m_library->GetPage(true)->GetList();
		base_list->Insert(const_cast<ee::Symbol*>(dynamic_cast<const ee::Symbol*>(base)));
	}
	if (const s2::Symbol* mask = sym->GetSymbol(false)) {
		ee::LibraryList* mask_list = m_library->GetPage(false)->GetList();
		mask_list->Insert(const_cast<ee::Symbol*>(dynamic_cast<const ee::Symbol*>(mask)));
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
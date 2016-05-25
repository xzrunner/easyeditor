#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
#include <ee/PropertySettingPanel.h>
#include <ee/FileType.h>

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
	if (ee::FileType::IsType(filepath, ee::FileType::e_mask)) {
		FileIO::Load(filepath, m_stage->GetSymbol());
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
	wxSplitterWindow* vert_split = new wxSplitterWindow(m_parent);
	wxSplitterWindow* hori_split = new wxSplitterWindow(vert_split);

	m_property = new ee::PropertySettingPanel(hori_split);
	m_library = new LibraryPanel(hori_split);

	m_stage = new StagePanel(vert_split, m_parent, m_property, m_library);

	m_property->SetEditPanel(m_stage->GetStageImpl());

	hori_split->SetSashGravity(0.55f);
	hori_split->SplitHorizontally(m_library, m_property);

	vert_split->SetSashGravity(0.2f);
	vert_split->SplitVertically(hori_split, m_stage);

	m_root = vert_split;
}

}
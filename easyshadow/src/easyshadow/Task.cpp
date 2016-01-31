#include "Task.h"

#include <easyshadow.h>

namespace eshadow
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	InitLayout();
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	ee::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filename)
{
	FileIO::LoadFromFile(filename, m_stage->GetSymbol());
	m_stage->LoadFromShadow();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void Task::Store(const char* filename) const
{
	FileIO::StoreToFile(filename, m_stage->GetSymbol());
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
	m_library = new LibraryPanel(parent);
	return m_library;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	return new StagePanel(parent, m_parent, m_library);
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	ToolbarPanel* toolbar = new ToolbarPanel(parent, static_cast<StagePanel*>(m_stage));
	return toolbar;
}

}
#include "Task.h"

#include <wx/splitter.h>

namespace eshape
{

Task::Task(wxFrame* parent)
{
	InitLayout(parent);
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
//	delete m_root;
}

void Task::load(const char* filepath)
{

}

void Task::store(const char* filepath) const
{

}

bool Task::isDirty() const
{
	return false;
}

void Task::clear()
{
// 	m_library->clear();
// 	m_stage->clear();
// 	m_library->Refresh();
// 	m_stage->Refresh();
}

void Task::getAllSprite(std::vector<const d2d::ISprite*>& sprites) const
{

}

const d2d::EditPanel* Task::getEditPanel() const
{
	return NULL;
}

void Task::InitLayout(wxFrame* parent)
{
	wxSplitterWindow* right_vert = new wxSplitterWindow(parent);
	wxSplitterWindow* left_vert = new wxSplitterWindow(right_vert);
	wxSplitterWindow* left_hori = new wxSplitterWindow(left_vert);

	d2d::LibraryPanel* library = new d2d::LibraryPanel(left_hori);
	library->addPage(new d2d::LibraryImagePage(library->getNotebook()));

	d2d::PropertySettingPanel* property = new d2d::PropertySettingPanel(left_hori);
	libshape::StagePanel* stage = new libshape::StagePanel(left_vert, parent, library);
	d2d::ToolbarPanel* toolbar = new libshape::ToolbarPanel(right_vert, property, stage);

	left_hori->SetSashGravity(0.8f);
	left_hori->SplitHorizontally(library, property);

	left_vert->SetSashGravity(0.15f);
	left_vert->SplitVertically(left_hori, stage);

	right_vert->SetSashGravity(0.85f);
	right_vert->SplitVertically(left_vert, toolbar);
}

}
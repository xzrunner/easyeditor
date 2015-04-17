#include "Task.h"

#include "view/StagePanel.h"

namespace lr
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
}

void Task::store(const char* filename) const
{
}

bool Task::isDirty() const
{
	return false;
}

void Task::clear()
{
	m_library->clear();
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

void Task::InitLayout()
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);
	wxSplitterWindow* left_hori_splitter = new wxSplitterWindow(left_splitter);

	d2d::PropertySettingPanel* property 
		= new d2d::PropertySettingPanel(left_hori_splitter);

	m_library = new d2d::LibraryPanel(left_hori_splitter);
	m_stage = new StagePanel(left_splitter, m_parent, property, m_library);
	m_library->setCanvas(m_stage->getCanvas());

	left_hori_splitter->SetSashGravity(0.6f);
	left_hori_splitter->SplitHorizontally(m_library, property);	

	left_splitter->SetSashGravity(0.2f);
	left_splitter->SplitVertically(left_hori_splitter, m_stage);

	d2d::ViewlistPanel* viewlist = new d2d::ViewlistPanel(right_splitter, m_stage, m_stage);
	m_stage->SetViewlist(viewlist);

	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_splitter, viewlist);

	m_root = right_splitter;
}

}
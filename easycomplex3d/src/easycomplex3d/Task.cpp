#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/LibraryPanel.h>
#include <ee/PropertySettingPanel.h>

#include <ee3/StagePanel.h>
#include <ee3/NodeArrangeOP.h>

#include <easycomplex3d.h>

#include <wx/splitter.h>

namespace ecomplex3d
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
	delete m_root;
}

void Task::Load(const char* filename)
{
//	Serializer::LoadFroimJson(filename, dynamic_cast<ee3::StagePanel*>(m_stage));
}

void Task::Store(const char* filename) const
{
//	Serializer::StoreToJson(filename, dynamic_cast<ee3::StagePanel*>(m_stage));
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
	//wxSplitterWindow* split = new wxSplitterWindow(parent);

	//m_library  = new LibraryPanel(split);
	//m_property = new ee3::PropertySettingPanel(split);

	//split->SetSashGravity(0.55f);
	//split->SplitHorizontally(m_library, m_property);

	//return split;

	m_library = new LibraryPanel(parent);
	return m_library;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	auto stage = new ee3::StagePanel(parent, m_parent, m_library);
	stage->SetCanvas(std::make_shared<StageCanvas>(stage, m_library));
	stage->SetEditOP(std::make_shared<ee3::NodeArrangeOP>(*stage));

	m_stage = stage;
//	m_property->SetEditPanel(m_stage->GetStageImpl());

	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	m_toolbar = new ecomplex3d::ToolbarPanel(parent, static_cast<ee3::StagePanel*>(m_stage));
	return m_toolbar;
}
	
}
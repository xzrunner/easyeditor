#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
#include <ee/FileType.h>
#include <ee/LibraryPanel.h>
#include <ee/LibraryImagePage.h>
#include <ee/PropertySettingPanel.h>

#include <easymesh.h>
#include <easycomplex.h>

namespace emesh
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
	if (!ee::FileType::IsType(filepath, ee::FILE_MESH)) {
		return;
	}

	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	Symbol* mesh_sym = static_cast<Symbol*>(sym);
	m_stage->SetMeshSymbol(mesh_sym);

	gum::MeshType type;
	s2::Mesh* mesh = mesh_sym->GetMesh();
	if (dynamic_cast<Network*>(mesh)) {
		type = gum::MESH_NETWORK;
	} else if (dynamic_cast<Strip*>(mesh)) {
		type = gum::MESH_STRIP;
	} else if (dynamic_cast<Skeleton*>(mesh)) {
		type = gum::MESH_SKELETON;
	}
	m_toolbar->SetEditType(type);

	m_library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
	sym->RemoveReference();
}

void Task::Store(const char* filepath) const
{
	FileIO::Store(filepath, m_stage->GetMeshSymbol());
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

	m_library = new ee::LibraryPanel(split);
	m_library->AddPage(new ee::LibraryImagePage(m_library->GetNotebook()));
	m_library->AddPage(new ecomplex::LibraryPage(m_library->GetNotebook()));

	m_property = new ee::PropertySettingPanel(split);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	StagePanel* stage = new StagePanel(parent, m_parent, NULL, m_library);
	m_stage = stage;
	m_property->SetEditPanel(m_stage->GetStageImpl());
	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	m_toolbar = new ToolbarPanel(parent, m_stage, true, NULL);
	return m_toolbar;
}

}
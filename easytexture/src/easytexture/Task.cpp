#include "Task.h"

#include <easytexture.h>

#include <ee/Config.h>
#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
#include <ee/PropertySettingPanel.h>
#include <ee/SettingData.h>

namespace etexture
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_library(NULL)
	, m_property(NULL)
	, m_stage(NULL)
	, m_toolbar(NULL)
{
	InitLayout();
	ee::Config::Instance()->GetSettings().visible_tex_edge = true;
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	ee::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filename)
{
	m_stage->GetSymbol()->LoadFromFile(filename);
}

void Task::Store(const char* filename) const
{
	FileSaver::Store(filename, m_stage->GetSymbol());
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
	wxSplitterWindow* right_splitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_vert_splitter = new wxSplitterWindow(right_splitter);
	wxSplitterWindow* left_hori_splitter = new wxSplitterWindow(left_vert_splitter);

	m_library = new LibraryPanel(left_hori_splitter);
	m_property = new ee::PropertySettingPanel(left_hori_splitter);

	left_hori_splitter->SetSashGravity(0.5f);
	left_hori_splitter->SplitHorizontally(m_library, m_property);

	StagePanel* stage;
	m_stage = stage = new StagePanel(left_vert_splitter, m_parent, m_library);
	m_property->SetEditPanel(m_stage->GetStageImpl());

	left_vert_splitter->SetSashGravity(0.2f);
	left_vert_splitter->SplitVertically(left_hori_splitter, m_stage);

	m_toolbar = new ToolbarPanel(right_splitter, m_stage);

	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_vert_splitter, m_toolbar);

	m_root = right_splitter;
}

}
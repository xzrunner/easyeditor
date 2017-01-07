#include "Task.h"

#include <ee/SymbolMgr.h>

#include <easyshader.h>

#include <wx/splitter.h>

namespace eshader
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_is_2d(false)
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
	Shader* shader = FileIO::LoadShader(filepath, m_stage->GetCanvas(), m_toolbar, m_is_2d);
	if (m_is_2d) {
		static_cast<StagePanel2D*>(m_stage)->SetShader(shader);
	} else {
		static_cast<StagePanel3D*>(m_stage)->SetShader(shader);
	}
}

void Task::Store(const char* filepath) const
{
	FileIO::StoreShader(filepath, m_toolbar);
}

bool Task::IsDirty() const
{
	return m_stage->IsEditDirty();
}

void Task::GetAllSprite(std::vector<ee::Sprite*>& sprs) const
{
}

const ee::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::InitLayout()
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);

	m_library = new LibraryPanel(left_splitter, m_is_2d);
	if (m_is_2d) {
		m_stage = new StagePanel2D(left_splitter, m_parent, m_library);
	} else {
		m_stage = new StagePanel3D(left_splitter, m_parent, m_library);
	}

	m_toolbar = new ToolbarPanel(right_splitter, m_stage);

	left_splitter->SetSashGravity(0.2f);
	left_splitter->SplitVertically(m_library, m_stage);

	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_splitter, m_toolbar);

	m_root = right_splitter;
}

}
#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
#include <ee/PropertySettingPanel.h>
#include <ee/Exception.h>

#include <easyparticle2d.h>

#include <wx/splitter.h>

namespace eparticle2d
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

	m_parent->SetTitle("EasyParticle");
}

void Task::Load(const char* filepath)
{
	if (!wxFileName::FileExists(filepath)) {
		throw ee::Exception("File: %s don't exist!", filepath);
	}
	FileIO::Load(filepath, m_stage->m_ps, m_toolbar);
}

void Task::Store(const char* filepath) const
{
	FileIO::Store(filepath, m_toolbar);
}

bool Task::IsDirty() const
{
	return m_stage->IsEditDirty();
}

void Task::GetAllSprite(std::vector<const ee::Sprite*>& sprites) const
{

}

const ee::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

// ee::StageCanvas* Task::getCanvas() const
// {
// 	return m_stage->getCanvas();
// }

void Task::InitLayout()
{
	wxSplitterWindow* rightSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftSplitter = new wxSplitterWindow(rightSplitter);
	wxSplitterWindow* left_hori_splitter = new wxSplitterWindow(leftSplitter);

	m_library = new LibraryPanel(left_hori_splitter);
	m_stage = new StagePanel(leftSplitter, m_parent, m_library);

	ee::PropertySettingPanel* property 
		= new ee::PropertySettingPanel(left_hori_splitter);
	property->SetEditPanel(m_stage->GetStageImpl());

//	m_toolbar = new ToolbarPanel(rightSplitter, m_library, m_stage, m_stage->GetStageData());
	m_toolbar = new ToolbarPanel(rightSplitter, m_library, m_stage);

	left_hori_splitter->SetSashGravity(0.6f);
	left_hori_splitter->SplitHorizontally(m_library, property);

	leftSplitter->SetSashGravity(0.2f);
	leftSplitter->SplitVertically(left_hori_splitter, m_stage);

	rightSplitter->SetSashGravity(0.7f);
	rightSplitter->SplitVertically(leftSplitter, m_toolbar);

	m_root = rightSplitter;
}

}
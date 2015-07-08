#include "Task.h"

#include <easyterrain3d.h>

namespace eterrain3d
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	initLayout();
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->Clear();
	d2d::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filename)
{
}

void Task::Store(const char* filename) const
{
}

bool Task::IsDirty() const
{
	return false;
}

void Task::Clear()
{
	m_library->Clear();
	m_stage->Clear();
	m_library->RefreshStage();
	m_stage->RefreshStage();
}

const d2d::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::initLayout()
{
	d2d::EditPanel* stage = new d2d::EditPanel(m_parent, m_parent);
	stage->SetCanvas(new libterrain3d::StageCanvas(stage));
	stage->SetEditOP(new e3d::ControlCameraOP(stage));
}

}
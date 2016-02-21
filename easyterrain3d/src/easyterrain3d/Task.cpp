#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
#include <ee/EditPanel.h>

#include <easyterrain3d.h>

namespace eterrain3d
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
}

void Task::Store(const char* filename) const
{
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
	ee::EditPanel* stage = new ee::EditPanel(m_parent, m_parent);
	stage->SetCanvas(new eterrain3d::StageCanvas(stage, stage->GetStageImpl()));
	stage->SetEditOP(new e3d::ControlCameraOP(stage, stage->GetStageImpl()));
}

}
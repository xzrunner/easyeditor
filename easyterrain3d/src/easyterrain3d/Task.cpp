#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/EditPanel.h>

#include <ee3/CamControlOP.h>
#include <ee3/StagePanel.h>

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
	auto stage = new ee3::StagePanel(m_parent, m_parent, nullptr);
	auto canvas = std::make_shared<eterrain3d::StageCanvas>(stage);
	stage->SetCanvas(canvas);
	stage->SetEditOP(std::make_shared<ee3::CamControlOP>(
		stage, stage->GetStageImpl(), canvas->GetCamera(), canvas->GetViewport(), stage->GetSubjectMgr()));
}

}
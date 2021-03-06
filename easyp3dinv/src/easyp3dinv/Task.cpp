#include "Task.h"

#include <ee/SymbolMgr.h>

#include <easyp3dinv.h>

namespace ep3dinv
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_stage(NULL)
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
	if (!m_stage->m_ps) {
		m_stage->m_ps = new ParticleSystem();
	}
	m_stage->m_ps->LoadFromFile(filename);
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
	m_stage = new StagePanel(m_parent, m_parent, NULL);
	m_root = m_stage;
}

}
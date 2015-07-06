#include "Task.h"

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
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
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

void Task::Clear()
{
	m_stage->clear();
	m_stage->Refresh();
}

const d2d::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::InitLayout()
{
	m_stage = new StagePanel(m_parent, m_parent);
	m_root = m_stage;
}

}
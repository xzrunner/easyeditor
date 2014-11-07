#include "Task.h"

#include <easyterrain.h>

namespace eterrain
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	initLayout();
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;
}

void Task::load(const char* filename)
{
}

void Task::store(const char* filename) const
{
}

bool Task::isDirty() const
{
	return false;
}

void Task::clear()
{
	m_library->clear();
	m_stage->clear();
	m_library->Refresh();
	m_stage->Refresh();
}

const d2d::EditPanel* Task::getEditPanel() const
{
	return m_stage;
}

void Task::initLayout()
{
	d2d::EditPanel* stage = new d2d::EditPanel(m_parent, m_parent);
	stage->setCanvas(new libterrain::StageCanvas(stage));
	stage->setEditOP(new e3d::ControlCameraOP(stage));
}

}
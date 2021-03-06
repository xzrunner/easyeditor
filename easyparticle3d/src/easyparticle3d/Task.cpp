#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/Exception.h>

#include <easyparticle3d.h>

namespace eparticle3d
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	InitLayout();

	m_property->SetPropertySetting(new SymbolPropertySetting(m_stage->m_ps));
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	delete m_root;

	m_parent->SetTitle("EasyParticle");
}

void Task::Load(const char* filepath)
{
	if (!wxFileName::FileExists(filepath)) {
		throw ee::Exception("File: %s don't exist!", filepath);
	}
	FileIO::Load(filepath, m_stage->m_ps, m_toolbar, m_library);
	m_property->SetPropertySetting(new SymbolPropertySetting(m_stage->m_ps));
	m_stage->m_ps->Start();
}

void Task::Store(const char* filepath) const
{
	FileIO::Store(filepath, m_stage->m_ps, m_toolbar);
}

bool Task::IsDirty() const
{
	return m_stage->IsEditDirty();
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

	right_split->SetSashGravity(0.7f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_library = new LibraryPanel(split);
	m_property = new PropertySettingPanel(split);
	
	split->SetSashGravity(0.7f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new StagePanel(parent, m_parent, m_library);
	m_property->SetStagePanel(m_stage);
	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	m_toolbar = new ToolbarPanel(parent, m_library, m_stage);
	return m_toolbar;
}

void Task::StoreAsAnim(const std::string& filepath) const
{
	m_stage->m_ps->StoreAnimRecord(filepath);
}

void Task::StoreAsInvert(const std::string& filepath) const
{
	m_stage->m_ps->StoreInvertRecord(filepath);
}

}
#include "Task.h"

#include <ee/Config.h>
#include <ee/SettingData.h>
#include <ee/SymbolMgr.h>
#include <ee/FileHelper.h>

#include <easyterrain2d.h>

#include <wx/splitter.h>

namespace eterrain2d
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
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
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();
	m_stage->Load(ee::FileHelper::GetFileDir(filename), value, m_library, m_toolbar);
}

void Task::Store(const char* filename) const
{
	Json::Value value;
	m_stage->Store(ee::FileHelper::GetFileDir(filename), value);
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
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
	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);

	m_library = new LibraryPanel(left_splitter);
	m_stage = new StagePanel(left_splitter, m_parent, m_library);

	left_splitter->SetSashGravity(0.2f);
	left_splitter->SplitVertically(m_library, m_stage);

	m_toolbar = new ToolbarPanel(right_splitter, static_cast<StagePanel*>(m_stage));
	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_splitter, m_toolbar);

	m_root = right_splitter;
}

}
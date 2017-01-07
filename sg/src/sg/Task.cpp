#include "Task.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"
#include "StageCanvas.h"

#include <ee/SymbolMgr.h>
#include <ee/FileHelper.h>

#include <easycomplex.h>

#include <wx/splitter.h>

namespace sg
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

void Task::Load(const char* filepath)
{
	FileIO::load(filepath, m_stage);
//	FormatFiles();
}

void Task::Store(const char* filepath) const
{
	FileIO::store(filepath, m_stage);
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

	left_split->SetSashGravity(0.12f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_library = new ee::LibraryPanel(split);

	m_property = new ee::PropertySettingPanel(split);

	split->SetSashGravity(0.75f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new StagePanel(parent, m_parent, m_library);
	m_property->SetEditPanel(m_stage->GetStageImpl());
	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	ToolbarPanel* toolbar = new ToolbarPanel(parent, m_library, m_property, m_stage);
	return toolbar;
}

void Task::FormatFiles()
{
	std::string dir("D:/projects/ejoy/svn/sanguo/Tools/sg-editor/map/Expedition");
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (filepath.find("_sg.json") != std::string::npos) {
			FileIO::load(filepath.c_str(), m_stage);
			FileIO::store(filepath.c_str(), m_stage);
//			Clear();
		}
	}
}

}
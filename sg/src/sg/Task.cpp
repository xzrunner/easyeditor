#include "Task.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"
#include "StageCanvas.h"

#include <easycomplex.h>

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
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;
}

void Task::load(const char* filepath)
{
	FileIO::load(filepath, m_stage);
//	FormatFiles();
}

void Task::store(const char* filepath) const
{
	FileIO::store(filepath, m_stage);
}

bool Task::isDirty() const
{
	return m_stage->isDirty();
}

void Task::clear()
{
	m_stage->clear();
	m_stage->Refresh();
}

const d2d::EditPanel* Task::getEditPanel() const
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

	m_library = new d2d::LibraryPanel(split);

	m_property = new d2d::PropertySettingPanel(split);

	split->SetSashGravity(0.75f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new StagePanel(parent, m_parent, m_library);
	m_library->SetCanvas(m_stage->getCanvas());
	m_property->SetEditPanel(m_stage);
	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	ToolbarPanel* toolbar = new ToolbarPanel(parent, m_library, m_property, m_stage);
	return toolbar;
}

void Task::FormatFiles()
{
	wxString dir("D:/projects/ejoy/svn/sanguo/Tools/sg-editor/map/Expedition");
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir.ToStdString(), files);
	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (filepath.Contains("_sg.json")) {
			FileIO::load(filepath, m_stage);
			FileIO::store(filepath, m_stage);
			clear();
		}
	}
}

}
#include "Task.h"

#include <drag2d.h>
#include <easyterrain2d.h>

namespace eterrain2d
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

void Task::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();
	m_stage->Load(d2d::FilenameTools::getFileDir(filename).ToStdString(), value, m_library, m_toolbar);
}

void Task::store(const char* filename) const
{
	Json::Value value;
	m_stage->Store(d2d::FilenameTools::getFileDir(filename).ToStdString(), value);
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

bool Task::isDirty() const
{
	return false;
}

void Task::clear()
{
	m_library->Clear();
	m_stage->clear();
	m_library->Refresh();
	m_stage->Refresh();
}

const d2d::EditPanel* Task::getEditPanel() const
{
	return m_stage;
}

void Task::InitLayout()
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);

	m_library = new LibraryPanel(left_splitter);
	m_stage = new StagePanel(left_splitter, m_parent, m_library);
	m_library->SetCanvas(m_stage->getCanvas());

	left_splitter->SetSashGravity(0.2f);
	left_splitter->SplitVertically(m_library, m_stage);

	m_toolbar = new ToolbarPanel(right_splitter, static_cast<StagePanel*>(m_stage));
	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_splitter, m_toolbar);

	m_root = right_splitter;
}

}
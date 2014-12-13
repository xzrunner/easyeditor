#include "Task.h"

#include <easyshader.h>
#include <wx/splitter.h>

namespace eshader
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

void Task::load(const char* filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

 	wxString dir = d2d::FilenameTools::getFileDir(filepath);
 	std::string vert_path = dir + "\\" + value["vert_path"].asString(),
 		frag_path = dir + "\\" + value["frag_path"].asString();
 	Shader* shader = new Shader(vert_path, frag_path);
 	shader->Load();
 	d2d::ShaderMgr::Instance()->SetSpriteShader(shader->GetShaderImpl());
 	m_stage->getCanvas()->resetViewport();

// 	d2d::SpriteShader* shader2 = new d2d::SpriteShader;
// 	shader2->Load();
// 	d2d::ShaderMgr::Instance()->SetSpriteShader(shader2);
// 	m_stage->getCanvas()->resetViewport();
}

void Task::store(const char* filepath) const
{
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

void Task::getAllSprite(std::vector<const d2d::ISprite*>& sprites) const
{
}

const d2d::EditPanel* Task::getEditPanel() const
{
	return m_stage;
}

void Task::initLayout()
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);

	m_library = new LibraryPanel(left_splitter);
	m_stage = new StagePanel(left_splitter, m_parent, m_library);
	m_library->setCanvas(m_stage->getCanvas());

	m_toolbar = new ToolbarPanel(right_splitter, m_stage);

	left_splitter->SetSashGravity(0.2f);
	left_splitter->SplitVertically(m_library, m_stage);

	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_splitter, m_toolbar);

	m_root = right_splitter;
}

}
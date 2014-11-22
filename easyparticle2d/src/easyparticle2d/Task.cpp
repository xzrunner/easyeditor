#include "Task.h"

#include <easyparticle2d.h>

namespace eparticle2d
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

	m_parent->SetTitle("EasyParticle");
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

	int version = value["version"].asInt();
	LoadPSSymbol(filename, value);
	m_toolbar->Load(value, version);
}

void Task::store(const char* filename) const
{
	Json::Value value;

	StorePSSymbol(filename, value);
	m_toolbar->Store(value);
	value["version"] = VERSION;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
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

// d2d::GLCanvas* Task::getCanvas() const
// {
// 	return m_stage->getCanvas();
// }

void Task::initLayout()
{
	wxSplitterWindow* rightSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftSplitter = new wxSplitterWindow(rightSplitter);
	wxSplitterWindow* left_hori_splitter = new wxSplitterWindow(leftSplitter);

	m_library = new LibraryPanel(left_hori_splitter);
	m_stage = new StagePanel(leftSplitter, m_parent, m_library);
	m_library->setCanvas(m_stage->getCanvas());

	d2d::PropertySettingPanel* property 
		= new d2d::PropertySettingPanel(left_hori_splitter);
	property->setPropertySetting(new PropertySetting(m_stage, &m_ps_name));

	m_toolbar = new ToolbarPanel(rightSplitter, m_library, m_stage, m_stage->GetStageData());

	left_hori_splitter->SetSashGravity(0.6f);
	left_hori_splitter->SplitHorizontally(m_library, property);

	leftSplitter->SetSashGravity(0.2f);
	leftSplitter->SplitVertically(left_hori_splitter, m_stage);

	rightSplitter->SetSashGravity(0.85f);
	rightSplitter->SplitVertically(leftSplitter, m_toolbar);

	m_root = rightSplitter;
}

void Task::StorePSSymbol(const char* filename, Json::Value& val) const
{
	ParticleSystem* ps = m_stage->GetStageData()->GetCurrPS();
	wxString dir = d2d::FilenameTools::getFileDir(filename) + "\\";
	val["symbol_path"] = d2d::FilenameTools::getRelativePath(dir,
		ps->GetSymbolFilePath()).ToStdString();
	val["name"] = m_ps_name;
}

void Task::LoadPSSymbol(const char* filename, const Json::Value& val)
{
	StageData* sd = m_stage->GetStageData();
	std::string dir = d2d::FilenameTools::getFileDir(filename);
	wxString path = d2d::FilenameTools::getAbsolutePath(dir, val["symbol_path"].asString());
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);
	sd->ChangePSSymbol(symbol);
	symbol->Release();

	m_ps_name = val["name"].asString();
}

}
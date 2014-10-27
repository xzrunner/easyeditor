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

void Task::loadFromFile(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	LoadPSSymbol(filename, value);
	m_toolbar->Load(value);
}

void Task::storeToFile(const char* filename) const
{
	Json::Value value;

	StorePSSymbol(filename, value);
	m_toolbar->Store(value);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void Task::clear()
{
	m_stage->clear();
	m_stage->Refresh();
}

d2d::GLCanvas* Task::getCanvas() const
{
	return m_stage->getCanvas();
}

void Task::initLayout()
{
	wxSplitterWindow* rightSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftSplitter = new wxSplitterWindow(rightSplitter);

	m_library = new LibraryPanel(leftSplitter);
	m_stage = new StagePanel(leftSplitter, m_parent, m_library);
	m_toolbar = new ToolbarPanel(rightSplitter, m_library, m_stage, 
		m_stage->GetParticleSystem());

	leftSplitter->SetSashGravity(0.2f);
	leftSplitter->SplitVertically(m_library, m_stage);

	rightSplitter->SetSashGravity(0.7f);
	rightSplitter->SplitVertically(leftSplitter, m_toolbar);

	m_root = rightSplitter;

// 	wxSize size = m_parent->GetSize();
// 	size.SetWidth(size.GetWidth() + 1);
// 	m_parent->SetSize(size);
}

void Task::StorePSSymbol(const char* filename, Json::Value& val) const
{
	ParticleSystem* ps = m_stage->GetParticleSystem();
	wxString dir = d2d::FilenameTools::getFileDir(filename) + "\\";
	val["symbol_path"] = d2d::FilenameTools::getRelativePath(dir,
		ps->GetSymbolFilePath()).ToStdString();
}

void Task::LoadPSSymbol(const char* filename, const Json::Value& val)
{
	ParticleSystem* ps = m_stage->GetParticleSystem();
	std::string dir = d2d::FilenameTools::getFileDir(filename);
	wxString path = d2d::FilenameTools::getAbsolutePath(dir, val["symbol_path"].asString());
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);
	ps->SetSymbol(symbol);
	symbol->release();
}

}
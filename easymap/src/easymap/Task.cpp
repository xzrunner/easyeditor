#include "Task.h"

#include <easymap.h>

namespace emap
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_library(NULL)
	, m_property(NULL)
	, m_stage(NULL)
	, m_viewlist(NULL)
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

	wxString dir = d2d::FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value spr_val = value["sprites"][i++];
	while (!spr_val.isNull()) {
		wxString filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->load(spr_val);
		symbol->Release();
		m_stage->insertSprite(sprite);
		spr_val = value["sprites"][i++];
	}

	m_library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());

	m_stage->resetCanvas();
}

void Task::store(const char* filename) const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	Json::Value value;
	wxString dir = d2d::FilenameTools::getFileDir(filename) + "\\";
	for (size_t i = 0; i < sprites.size(); ++i) {
		d2d::ISprite* spr = sprites[i];
		value["sprites"][i]["filepath"] = d2d::FilenameTools::getRelativePath(dir,
			spr->getSymbol().getFilepath()).ToStdString();
		spr->store(value["sprites"][i]);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));
	if (fout.fail()) {
		throw d2d::Exception("Can't save file: %s !", filename);
	}
	writer.write(fout, value);
	fout.close();
}

bool Task::isDirty() const
{
	return false;
}

void Task::clear()
{
	m_viewlist->clear();

	m_library->clear();
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
	wxSplitterWindow* left_vert_splitter = new wxSplitterWindow(right_splitter);
	wxSplitterWindow* left_hori_splitter = new wxSplitterWindow(left_vert_splitter);

	m_library = new LibraryPanel(left_hori_splitter);
	m_property = new d2d::PropertySettingPanel(left_hori_splitter);

	left_hori_splitter->SetSashGravity(0.5f);
	left_hori_splitter->SplitHorizontally(m_library, m_property);

	StagePanel* stage;
	m_stage = stage = new StagePanel(left_vert_splitter, m_parent, m_library, m_property);
	m_library->setCanvas(m_stage->getCanvas());

	left_vert_splitter->SetSashGravity(0.2f);
	left_vert_splitter->SplitVertically(left_hori_splitter, m_stage);

//	ToolbarPanel* toolbar = new ToolbarPanel(right_splitter, static_cast<StagePanel*>(m_stage));
	m_viewlist = new d2d::ViewlistPanel(right_splitter, m_stage, stage, m_property);
	stage->SetViewlist(m_viewlist);

	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_vert_splitter, m_viewlist);

	m_root = right_splitter;
}

}
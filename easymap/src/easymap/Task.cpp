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
	d2d::SymbolMgr::Instance()->Clear();
	d2d::BitmapMgr::Instance()->Clear();
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

	wxString dir = d2d::FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value spr_val = value["sprites"][i++];
	while (!spr_val.isNull()) {
		std::string filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->Load(spr_val);
		symbol->Release();
		d2d::InsertSpriteSJ::Instance()->Insert(sprite);
		spr_val = value["sprites"][i++];
	}

	m_library->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());

	m_stage->ResetCanvas();
}

void Task::Store(const char* filename) const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	Json::Value value;
	wxString dir = d2d::FilenameTools::getFileDir(filename) + "\\";
	for (size_t i = 0; i < sprites.size(); ++i) {
		d2d::ISprite* spr = sprites[i];
		value["sprites"][i]["filepath"] = d2d::FilenameTools::getRelativePath(dir,
			spr->GetSymbol().GetFilepath()).ToStdString();
		spr->Store(value["sprites"][i]);
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

bool Task::IsDirty() const
{
	return false;
}

void Task::GetAllSprite(std::vector<const d2d::ISprite*>& sprites) const
{
	m_stage->TraverseSprites(d2d::FetchAllVisitor<const d2d::ISprite>(sprites));
}

const d2d::EditPanel* Task::GetEditPanel() const
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
	m_property->SetEditPanel(m_stage->GetStageImpl());
	m_library->SetCanvas(m_stage->GetCanvas());

	left_vert_splitter->SetSashGravity(0.2f);
	left_vert_splitter->SplitVertically(left_hori_splitter, m_stage);

//	ToolbarPanel* toolbar = new ToolbarPanel(right_splitter, static_cast<StagePanel*>(m_stage));
	m_viewlist = new d2d::ViewlistPanel(right_splitter);

	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_vert_splitter, m_viewlist);

	m_root = right_splitter;
}

}
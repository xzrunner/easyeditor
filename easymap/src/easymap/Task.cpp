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

	wxString dir = ee::FileHelper::GetFileDir(filename);

	int i = 0;
	Json::Value spr_val = value["sprites"][i++];
	while (!spr_val.isNull()) {
		std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, spr_val);
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		sprite->Load(spr_val);
		symbol->Release();
		ee::InsertSpriteSJ::Instance()->Insert(sprite);
		spr_val = value["sprites"][i++];
	}

	m_library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

void Task::Store(const char* filename) const
{
	std::vector<ee::Sprite*> sprites;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));

	Json::Value value;
	wxString dir = ee::FileHelper::GetFileDir(filename) + "\\";
	for (size_t i = 0; i < sprites.size(); ++i) {
		ee::Sprite* spr = sprites[i];
		value["sprites"][i]["filepath"] = ee::FileHelper::GetRelativePath(dir,
			spr->GetSymbol().GetFilepath()).ToStdString();
		spr->Store(value["sprites"][i]);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));
	if (fout.fail()) {
		throw ee::Exception("Can't save file: %s !", filename);
	}
	writer.write(fout, value);
	fout.close();
}

bool Task::IsDirty() const
{
	return false;
}

void Task::GetAllSprite(std::vector<const ee::Sprite*>& sprites) const
{
	m_stage->TraverseSprites(ee::FetchAllVisitor<const ee::Sprite>(sprites));
}

const ee::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::InitLayout()
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_vert_splitter = new wxSplitterWindow(right_splitter);
	wxSplitterWindow* left_hori_splitter = new wxSplitterWindow(left_vert_splitter);

	m_library = new LibraryPanel(left_hori_splitter);

	m_property = new ee::PropertySettingPanel(left_hori_splitter);

	left_hori_splitter->SetSashGravity(0.5f);
	left_hori_splitter->SplitHorizontally(m_library, m_property);

	StagePanel* stage;
	m_stage = stage = new StagePanel(left_vert_splitter, m_parent, m_library, m_property);
	m_property->SetEditPanel(m_stage->GetStageImpl());

	left_vert_splitter->SetSashGravity(0.2f);
	left_vert_splitter->SplitVertically(left_hori_splitter, m_stage);

//	ToolbarPanel* toolbar = new ToolbarPanel(right_splitter, static_cast<StagePanel*>(m_stage));
	m_viewlist = new ee::ViewlistPanel(right_splitter);

	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_vert_splitter, m_viewlist);

	m_root = right_splitter;
}

}
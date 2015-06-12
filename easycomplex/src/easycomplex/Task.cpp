#include "Task.h"
#include "FileIO.h"

#include <easycomplex.h>
#include <easycoco.h>

namespace ecomplex
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

	m_library->LoadFromConfig();
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;
}

void Task::load(const char* filepath)
{
	if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
		FileIO::load(this, filepath);
		LoadGroupTree(filepath);
		m_stage->getCanvas()->resetViewport();
	}
}

void Task::store(const char* filepath) const
{
	FileIO::store(this, filepath);
	StoreGroupTree(filepath);
	m_stage->onSave();
}

bool Task::isDirty() const
{
	return m_stage->isDirty();
}

void Task::clear()
{
	//	d2d::SymbolMgr::Instance()->clear();

	m_viewlist->Clear();

	m_library->Clear();
	m_stage->clear();
	m_library->Refresh();
	m_stage->Refresh();
}

void Task::getAllSprite(std::vector<const d2d::ISprite*>& sprites) const
{
	m_stage->traverseSprites(d2d::FetchAllVisitor<const d2d::ISprite>(sprites));
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

	left_split->SetSashGravity(0.2f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_library = new ecomplex::LibraryPanel(split);

	m_property = new d2d::PropertySettingPanel(split);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new ecomplex::StagePanel(parent, m_parent, m_property, m_library, &m_view_panel_mgr);

	m_view_panel_mgr.AddPanel(m_stage);
	m_library->SetCanvas(m_stage->getCanvas());
	m_property->SetPropertySetting(new ecomplex::SymbolPropertySetting(m_stage, m_stage->getSymbol()));

	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_viewlist = new d2d::ViewlistPanel(split, m_stage, m_stage, m_property, &m_view_panel_mgr);
	m_view_panel_mgr.AddPanel(m_viewlist);

	m_grouptree = new d2d::GroupTreePanel(split, m_stage, &m_view_panel_mgr);
	m_view_panel_mgr.AddPanel(m_grouptree);

	split->SetSashGravity(0.5f);
	split->SplitHorizontally(m_viewlist, m_grouptree);

	return split;
}

void Task::LoadGroupTree(const char* filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_grouptree->LoadFromFile(value["group"]);

	std::map<std::string, std::vector<d2d::ISprite*> > map_actions;
	std::vector<d2d::ISprite*> others;
	libcoco::Utility::GroupSpritesFromTag(m_stage->getSymbol()->m_sprites, map_actions, others);
	m_grouptree->InitGroups(map_actions);
}

void Task::StoreGroupTree(const char* filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_grouptree->StoreToFile(value["group"]);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));
	writer.write(fout, value);
	fout.close();
}

}
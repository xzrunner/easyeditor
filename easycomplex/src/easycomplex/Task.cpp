#include "Task.h"
#include "FileIO.h"

#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
#include <ee/FileType.h>
#include <ee/FetchAllVisitor.h>
#include <ee/ViewlistPanel.h>
#include <ee/GroupTreePanel.h>

#include <easycomplex.h>
#include <easycoco.h>

#include <fstream>

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
	ee::SymbolMgr::Instance()->Clear();
	ee::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filepath)
{
	if (ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
		FileIO::load(this, filepath);
		LoadGroupTree(filepath);
	} else if (ee::FileType::IsType(filepath, ee::FileType::e_psd)) {
		FileIO::load(this, filepath);
	}
}

void Task::Store(const char* filepath) const
{
	FileIO::store(this, filepath);
	StoreGroupTree(filepath);
	m_stage->OnSave();
}

bool Task::IsDirty() const
{
	return m_stage->IsEditDirty();
}

void Task::GetAllSprite(std::vector<const ee::Sprite*>& sprs) const
{
	std::vector<ee::Sprite*> tmp;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(tmp));
	for (int i = 0, n = tmp.size(); i < n; ++i) {
		sprs.push_back(tmp[i]);
	}
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
	m_property = new ecomplex::PropertySettingPanel(split);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new ecomplex::StagePanel(parent, m_parent, m_property, m_library);
	m_property->SetEditPanel(m_stage->GetStageImpl());
	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_viewlist = new ee::ViewlistPanel(split);
	m_viewlist->SetListImpl(new ViewlistListImpl(m_stage, m_stage->GetStageImpl(), m_stage));

	m_grouptree = new ee::GroupTreePanel(split, m_stage, m_stage->GetKeyState());

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

	if (!value["group"].isNull()) {
		m_grouptree->LoadFromFile(value["group"]);
	} else {
		std::map<std::string, std::vector<ee::Sprite*> > map_actions;
		std::vector<ee::Sprite*> others;
		ecoco::Utility::GroupSpritesFromTag(m_stage->GetSymbol()->GetChildren(), map_actions, others);
		m_grouptree->InitGroups(map_actions);
	}
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
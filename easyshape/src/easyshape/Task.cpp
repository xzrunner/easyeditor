#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/SymbolFile.h>
#include <ee/FileHelper.h>
#include <ee/LibraryImagePage.h>

#include <easyshape.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easytexture.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

#include <wx/splitter.h>

namespace eshape
{

Task::Task(wxFrame* parent)
{
	InitLayout(parent);
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	ee::BitmapMgr::Instance()->Clear();
//	delete m_root;
}

void Task::Load(const char* filepath)
{
	int type = ee::SymbolFile::Instance()->Type(filepath);
	switch (type)
	{
	case s2::SYM_SHAPE:
		m_stage->LoadFromFile(filepath);
		break;
	case s2::SYM_IMAGE: case s2::SYM_COMPLEX: case s2::SYM_TEXTURE:
		{
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			m_stage->SetSymbolBG(sym);
			sym->RemoveReference();
		}
		break;
	}
}

void Task::Store(const char* filepath) const
{
	std::string fixed = filepath;
	if (ee::SymbolFile::Instance()->Type(fixed) != s2::SYM_SHAPE) {
		std::string tag = ee::SymbolFile::Instance()->Tag(s2::SYM_SHAPE);
		fixed = ee::FileHelper::GetFilenameAddTag(fixed, tag, "json");
	}
	m_stage->StoreToFile(fixed.c_str());
}

bool Task::IsDirty() const
{
	return true;
}

void Task::GetAllSprite(std::vector<ee::Sprite*>& sprs) const
{
}

const ee::EditPanel* Task::GetEditPanel() const
{
	return NULL;
}

void Task::InitLayout(wxFrame* parent)
{
	wxSplitterWindow* right_vert = new wxSplitterWindow(parent);
	wxSplitterWindow* left_vert = new wxSplitterWindow(right_vert);
	wxSplitterWindow* left_hori = new wxSplitterWindow(left_vert);

	m_library = new ee::LibraryPanel(left_hori);
	wxWindow* nb = m_library->GetNotebook();
	m_library->AddPage(new ee::LibraryImagePage(nb));
	m_library->AddPage(new ecomplex::LibraryPage(nb));
	m_library->AddPage(new libanim::LibraryPage(nb));
	m_library->AddPage(new etexture::LibraryPage(nb));

	ee::PropertySettingPanel* property = new ee::PropertySettingPanel(left_hori);

	m_stage = new eshape::StagePanel(left_vert, parent, m_library);
	property->SetEditPanel(m_stage->GetStageImpl());

	ee::ToolbarPanel* toolbar = new eshape::ToolbarPanel(right_vert, property, m_stage);

	left_hori->SetSashGravity(0.8f);
	left_hori->SplitHorizontally(m_library, property);

	left_vert->SetSashGravity(0.15f);
	left_vert->SplitVertically(left_hori, m_stage);

	right_vert->SetSashGravity(0.85f);
	right_vert->SplitVertically(left_vert, toolbar);
}

}
#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/FileType.h>
#include <ee/FileHelper.h>
#include <ee/LibraryImagePage.h>

#include <easyshape.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easytexture.h>

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
	ee::FileType::Type type = ee::FileType::GetType(filepath);
	if (type == ee::FileType::e_shape) {
		m_stage->LoadFromFile(filepath);		
	} else if (type == ee::FileType::e_image 
		|| type == ee::FileType::e_complex
		|| type == ee::FileType::e_texture) {
		ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		m_stage->SetSymbolBG(sym);
		sym->RemoveReference();
	}
}

void Task::Store(const char* filepath) const
{
	std::string fixed = filepath;
	ee::FileType::Type type = ee::FileType::GetType(fixed);
	if (type != ee::FileType::e_shape) {
		std::string tag = ee::FileType::GetTag(ee::FileType::e_shape);
		fixed = ee::FileHelper::GetFilenameAddTag(fixed, tag, "json");
	}
	m_stage->StoreToFile(fixed.c_str());
}

bool Task::IsDirty() const
{
	return true;
}

void Task::GetAllSprite(std::vector<const ee::Sprite*>& sprs) const
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
	m_library->AddPage(new eanim::LibraryPage(nb));
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
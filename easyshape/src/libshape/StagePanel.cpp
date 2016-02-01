#include "StagePanel.h"
#include "StageCanvas.h"
#include "FileIO.h"
#include "Symbol.h"
#include "ToolBarPanel.h"

namespace eshape
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: EditPanel(parent, frame)
	, MultiShapesImpl()
	, m_toolbar(NULL)
{
	SetCanvas(new StageCanvas(this));
	m_symbol = new Symbol;

	SetDropTarget(new DropTarget(this, library));

	InitSubjects();
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, ee::Sprite* edited, 
					   const ee::MultiSpritesImpl* bg_sprites)
	: EditPanel(parent, frame)
	, MultiShapesImpl()
	, m_toolbar(NULL)
{
	SetCanvas(new StageCanvas(this, glctx, edited, bg_sprites));
	m_symbol = (Symbol*)(&edited->GetSymbol());
	if (m_symbol) {
		m_symbol->Retain();
	}

	InitSubjects();
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   Symbol* symbol/*, ee::LibraryPanel* library*/)
	: EditPanel(parent, frame)
	, MultiShapesImpl()
	, m_toolbar(NULL)
{
	SetCanvas(new StageCanvas(this));
	m_symbol = symbol;
	if (m_symbol) {
		m_symbol->Retain();
	}

//	SetDropTarget(new DropTarget(this, library));

	InitSubjects();
}

StagePanel::~StagePanel()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

void StagePanel::TraverseShapes(ee::Visitor& visitor, ee::DataTraverseType type/* = ee::DT_ALL*/) const
{
	if (m_symbol) {
		m_symbol->Traverse(visitor);
	}
}

const ee::Symbol& StagePanel::GetSymbol() const
{
	return *m_symbol;
}

void StagePanel::LoadFromFile(const char* filename)
{
	if (m_symbol) {
		m_symbol->LoadFromFile(filename);
	}
}

void StagePanel::StoreToFile(const char* filename) const
{
	if (m_symbol) {
		m_symbol->StoreToFile(filename);
	}
}

void StagePanel::SetSymbolBG(ee::Symbol* symbol)
{
	if (m_symbol) {
		m_symbol->SetBG(symbol);
		if (m_toolbar) {
			m_toolbar->SelectSuitableEditOP();
		}
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

//void StagePanel::loadShapes()
//{
// 	if (!m_sprite)
// 	{
// 		std::string filepath = m_symbol.getFilepath();
// 		if (!ee::FileType::isType(filepath, ee::FileType::e_shape))
// 		{
// 			filepath = ee::FileHelper::getFilePathExceptExtension(filepath);
// 			filepath = filepath + "_" + ee::FileType::GetTag(ee::FileType::e_shape) + ".json";
// 		}
// 
// 		if (ee::FileHelper::isExist(filepath))
// 		{
// 			libshape::FileAdapter adapter(m_shapes);
// 			adapter.load(filepath.c_str());
// 		}
// 	}
//}
//
//void StagePanel::storeShapes() const
//{
//	if (m_shapes.empty()) return;
//
//	if (!m_sprite)
//	{
//		std::string filepath = m_symbol.getFilepath();
//		if (filepath.empty())
//			filepath = m_symbol.getName() + "_" + ee::FileType::GetTag(ee::FileType::e_shape) + ".json";
//		if (!ee::FileType::isType(filepath, ee::FileType::e_shape))
//		{
//			filepath = ee::FileHelper::getFilePathExceptExtension(filepath);
//			filepath = filepath + "_" + ee::FileType::GetTag(ee::FileType::e_shape) + ".json";
//		}
//		else
//		{
//			Symbol* symbol = static_cast<Symbol*>(const_cast<ee::Symbol*>(&m_symbol));
//
//			std::vector<ee::Shape*>& shapes = symbol->m_shapes;
//
//			for (size_t i = 0, n = shapes.size(); i < n; ++i)
//				shapes[i]->Release();
//			shapes.clear();
//
//			for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
//				m_shapes[i]->Retain();
//			copy(m_shapes.begin(), m_shapes.end(), back_inserter(shapes));
//		}
//
////		if (ee::FileHelper::isExist(filepath))
//		{
//			libshape::FileAdapter adapter(m_shapes);
//			adapter.store(filepath.c_str());
//		}
//	}
//}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::MultiShapesImpl::OnNotify(sj_id, ud);

	if (!m_symbol) {
		return;
	}

	switch (sj_id)
	{
	case ee::MSG_REMOVE_SHAPE:
		if (m_symbol->Remove((ee::Shape*)ud)) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case ee::MSG_INSERT_SHAPE:
		if (m_symbol->Add((ee::Shape*)ud)) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case ee::MSG_CLEAR_SHAPE:
		if (m_symbol->Clear()) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	}
}

void StagePanel::InitSubjects()
{
	RegistSubject(ee::RemoveShapeSJ::Instance());
	RegistSubject(ee::InsertShapeSJ::Instance());
	RegistSubject(ee::ClearShapeSJ::Instance());
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::DragSymbolTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::DropTarget::
DropTarget(StagePanel* stage, ee::LibraryPanel* library)
	: ee::CombinedDropTarget(stage)
	, m_stage(stage)
	, m_library(library)
{
}

void StagePanel::DropTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	ee::Symbol* symbol = m_library->GetSymbol(index);
	if (symbol) {
		m_stage->SetSymbolBG(symbol);
	}
}

void StagePanel::DropTarget::
OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	if (filenames.IsEmpty()) {
		return;
	}

	std::string filename = filenames[0].ToStdString();
	ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filename);
	symbol->RefreshThumbnail(filename);
	bool success = m_library->AddSymbol(symbol);
	if (success) {
		m_stage->SetSymbolBG(symbol);
	}
	symbol->Release();
}

}
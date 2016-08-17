#include "StagePanel.h"
#include "StageCanvas.h"
#include "FileIO.h"
#include "Symbol.h"
#include "ToolBarPanel.h"

#include <ee/Sprite.h>
#include <ee/panel_msg.h>
#include <ee/subject_id.h>
#include <ee/shape_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/LibraryPanel.h>
#include <ee/SymbolMgr.h>

namespace eshape
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: EditPanel(parent, frame)
	, MultiShapesImpl()
	, m_toolbar(NULL)
{
	SetCanvas(new StageCanvas(this));
	m_sym = new Symbol;

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
	m_sym = dynamic_cast<Symbol*>(edited->GetSymbol());
	if (m_sym) {
		m_sym->AddReference();
	}

	InitSubjects();
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   Symbol* sym/*, ee::LibraryPanel* library*/)
	: EditPanel(parent, frame)
	, MultiShapesImpl()
	, m_toolbar(NULL)
{
	SetCanvas(new StageCanvas(this));
	m_sym = sym;
	if (m_sym) {
		m_sym->AddReference();
	}

//	SetDropTarget(new DropTarget(this, library));

	InitSubjects();
}

StagePanel::~StagePanel()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void StagePanel::TraverseShapes(ee::Visitor<ee::Shape>& visitor, ee::DataTraverseType type/* = ee::DT_ALL*/) const
{
	if (m_sym) {
		m_sym->Traverse(visitor);
	}
}

const ee::Symbol& StagePanel::GetSymbol() const
{
	return *m_sym;
}

void StagePanel::LoadFromFile(const char* filename)
{
	if (m_sym) {
		m_sym->LoadFromFile(filename);
	}
}

void StagePanel::StoreToFile(const char* filename) const
{
	if (m_sym) {
		m_sym->StoreToFile(filename);
	}
}

void StagePanel::SetSymbolBG(ee::Symbol* sym)
{
	if (m_sym) {
		m_sym->SetBG(sym);
		if (m_toolbar) {
			m_toolbar->SelectSuitableEditOP();
		}
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

//void StagePanel::loadShapes()
//{
// 	if (!m_spr)
// 	{
// 		std::string filepath = m_sym.getFilepath();
// 		if (!ee::FileType::isType(filepath, ee::FileType::e_shape))
// 		{
// 			filepath = ee::FileHelper::getFilePathExceptExtension(filepath);
// 			filepath = filepath + "_" + ee::FileType::GetTag(ee::FileType::e_shape) + ".json";
// 		}
// 
// 		if (ee::FileHelper::isExist(filepath))
// 		{
// 			eshape::FileAdapter adapter(m_shapes);
// 			adapter.load(filepath.c_str());
// 		}
// 	}
//}
//
//void StagePanel::storeShapes() const
//{
//	if (m_shapes.empty()) return;
//
//	if (!m_spr)
//	{
//		std::string filepath = m_sym.getFilepath();
//		if (filepath.empty())
//			filepath = m_sym.getName() + "_" + ee::FileType::GetTag(ee::FileType::e_shape) + ".json";
//		if (!ee::FileType::isType(filepath, ee::FileType::e_shape))
//		{
//			filepath = ee::FileHelper::getFilePathExceptExtension(filepath);
//			filepath = filepath + "_" + ee::FileType::GetTag(ee::FileType::e_shape) + ".json";
//		}
//		else
//		{
//			Symbol* sym = static_cast<Symbol*>(const_cast<ee::Symbol*>(&m_sym));
//
//			std::vector<ee::Shape*>& shapes = sym->m_shapes;
//
//			for (size_t i = 0, n = shapes.size(); i < n; ++i)
//				shapes[i]->RemoveReference();
//			shapes.clear();
//
//			for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
//				m_shapes[i]->AddReference();
//			copy(m_shapes.begin(), m_shapes.end(), back_inserter(shapes));
//		}
//
////		if (ee::FileHelper::isExist(filepath))
//		{
//			eshape::FileAdapter adapter(m_shapes);
//			adapter.store(filepath.c_str());
//		}
//	}
//}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::MultiShapesImpl::OnNotify(sj_id, ud);

	if (!m_sym) {
		return;
	}

	switch (sj_id)
	{
	case ee::MSG_REMOVE_SHAPE:
		if (m_sym->Remove((ee::Shape*)ud)) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case ee::MSG_INSERT_SHAPE:
		if (m_sym->Add((ee::Shape*)ud)) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case ee::MSG_CLEAR_SHAPE:
		if (m_sym->Clear()) {
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

	ee::Symbol* sym = m_library->GetSymbol(index);
	if (sym) {
		m_stage->SetSymbolBG(sym);
	}
}

void StagePanel::DropTarget::
OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	if (filenames.IsEmpty()) {
		return;
	}

	std::string filename = filenames[0].ToStdString();
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filename);
	sym->RefreshThumbnail(filename);
	bool success = m_library->AddSymbol(sym);
	if (success) {
		m_stage->SetSymbolBG(sym);
	}
	sym->RemoveReference();
}

}
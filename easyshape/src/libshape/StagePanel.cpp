#include "StagePanel.h"
#include "StageCanvas.h"
#include "FileAdapter.h"
#include "Symbol.h"

namespace libshape
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: EditPanel(parent, frame)
	, MultiShapesImpl(parent)
{
	m_canvas = new StageCanvas(this);
	m_symbol = new Symbol;

	SetDropTarget(new DragSymbolTarget(this, library));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   Symbol* symbol, d2d::LibraryPanel* library)
	: EditPanel(parent, frame)
	, MultiShapesImpl(parent)
{
	m_canvas = new StageCanvas(this);
	m_symbol = symbol;
	if (m_symbol) {
		m_symbol->Retain();
	}

	SetDropTarget(new DragSymbolTarget(this, library));
}

StagePanel::~StagePanel()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

void StagePanel::clear()
{
	EditPanel::clear();
	clearShapes();
}

void StagePanel::traverseShapes(d2d::IVisitor& visitor, d2d::TraverseType type/* = d2d::e_allExisting*/) const
{
	if (m_symbol) {
		m_symbol->Traverse(visitor);
	}
}

void StagePanel::removeShape(d2d::IShape* shape)
{
	if (m_symbol) {
		m_symbol->Remove(shape);
	}
}

void StagePanel::insertShape(d2d::IShape* shape)
{
	if (m_symbol) {
		m_symbol->Add(shape);
	}
}

void StagePanel::clearShapes()
{
	if (m_symbol) {
		m_symbol->Clear();
	}
}

const d2d::ISymbol& StagePanel::GetSymbol() const
{
	return *m_symbol;
}

void StagePanel::SetSymbolBG(d2d::ISymbol* symbol)
{
	if (m_symbol) {
		m_symbol->SetBG(symbol);
	}
}

//void StagePanel::loadShapes()
//{
// 	if (!m_sprite)
// 	{
// 		wxString filepath = m_symbol.getFilepath();
// 		if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_shape))
// 		{
// 			filepath = d2d::FilenameTools::getFilePathExceptExtension(filepath);
// 			filepath = filepath + "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
// 		}
// 
// 		if (d2d::FilenameTools::isExist(filepath))
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
//		wxString filepath = m_symbol.getFilepath();
//		if (filepath.empty())
//			filepath = m_symbol.getName() + "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
//		if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_shape))
//		{
//			filepath = d2d::FilenameTools::getFilePathExceptExtension(filepath);
//			filepath = filepath + "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
//		}
//		else
//		{
//			Symbol* symbol = static_cast<Symbol*>(const_cast<d2d::ISymbol*>(&m_symbol));
//
//			std::vector<d2d::IShape*>& shapes = symbol->m_shapes;
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
////		if (d2d::FilenameTools::isExist(filepath))
//		{
//			libshape::FileAdapter adapter(m_shapes);
//			adapter.store(filepath.c_str());
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::DragSymbolTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::DragSymbolTarget::
DragSymbolTarget(StagePanel* stage, d2d::LibraryPanel* library)
	: m_stage(stage)
	, m_library(library)
{
}

bool StagePanel::DragSymbolTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	d2d::ISymbol* symbol = m_library->getSymbol(index);
	if (symbol) {
		m_stage->SetSymbolBG(symbol);
	}
	
	return true;
}
}
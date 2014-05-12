#include "StagePanel.h"
#include "StageCanvas.h"
#include "FileAdapter.h"
#include "Symbol.h"

namespace libshape
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, d2d::ISymbol* symbol)
	: EditPanel(parent, frame)
	, MultiShapesImpl(parent)
	, m_symbol(*symbol)
	, m_sprite(NULL)
{
	m_canvas = new StageCanvas(this);
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, d2d::ISprite* sprite)
	: EditPanel(parent, frame)
	, MultiShapesImpl(parent)
	, m_symbol(sprite->getSymbol())
	, m_sprite(sprite)
{
	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
	clear();
}

void StagePanel::clear()
{
	EditPanel::clear();
	clearShapes();
}

void StagePanel::traverseShapes(d2d::IVisitor& visitor, 
							 d2d::TraverseType type/* = d2d::e_allExisting*/) const
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
	{
		bool hasNext;
		visitor.visit(m_shapes[i], hasNext);
		if (!hasNext) break;
	}
}

void StagePanel::removeShape(d2d::IShape* shape)
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
	{
		if (m_shapes[i] == shape)
		{
			m_shapes.erase(m_shapes.begin() + i);
			shape->release();
			break;
		}
	}
}

void StagePanel::insertShape(d2d::IShape* shape)
{
	m_shapes.push_back(shape);
}

void StagePanel::clearShapes()
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		m_shapes[i]->release();
	m_shapes.clear();
}

void StagePanel::loadShapes()
{
	if (!m_sprite)
	{
		wxString filepath = m_symbol.getFilepath();
		if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_shape))
		{
			filepath = d2d::FilenameTools::getFilePathExceptExtension(filepath);
			filepath = filepath + "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
		}

		if (d2d::FilenameTools::isExist(filepath))
		{
			libshape::FileAdapter adapter(m_shapes);
			adapter.load(filepath.c_str());
		}
	}
}

void StagePanel::storeShapes() const
{
	if (m_shapes.empty()) return;

	if (!m_sprite)
	{
		wxString filepath = m_symbol.getFilepath();
		if (filepath.empty())
			filepath = m_symbol.getName() + "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
		if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_shape))
		{
			filepath = d2d::FilenameTools::getFilePathExceptExtension(filepath);
			filepath = filepath + "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
		}
		else
		{
			Symbol* symbol = static_cast<Symbol*>(const_cast<d2d::ISymbol*>(&m_symbol));

			std::vector<d2d::IShape*>& shapes = symbol->shapes;

			for (size_t i = 0, n = shapes.size(); i < n; ++i)
				shapes[i]->release();
			shapes.clear();

			for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
				m_shapes[i]->retain();
			copy(m_shapes.begin(), m_shapes.end(), back_inserter(shapes));
		}

//		if (d2d::FilenameTools::isExist(filepath))
		{
			libshape::FileAdapter adapter(m_shapes);
			adapter.store(filepath.c_str());
		}
	}
}

}
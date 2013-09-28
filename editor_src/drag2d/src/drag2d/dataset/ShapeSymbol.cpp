#include "ShapeSymbol.h"

#include "common/ShapeFileAdapter.h"
#include "dataset/PolygonShape.h"

namespace d2d
{

ShapeSymbol::ShapeSymbol()
{
}

ShapeSymbol::ShapeSymbol(const ShapeSymbol& symbol)
{
	m_filepath = symbol.m_filepath;
	shapes.reserve(symbol.shapes.size());
	for (size_t i = 0, n = symbol.shapes.size(); i < n; ++i)
		shapes.push_back(symbol.shapes[i]->clone());
}

ShapeSymbol::~ShapeSymbol()
{
	clear();
}

ShapeSymbol* ShapeSymbol::clone()
{
	return new ShapeSymbol(*this); 
}

void ShapeSymbol::loadFromTextFile(std::ifstream& fin)
{

}

void ShapeSymbol::storeToTextFile(std::ofstream& fout) const
{

}

void ShapeSymbol::reloadTexture() const
{

}

void ShapeSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
		shapes[i]->draw();
}

float ShapeSymbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

float ShapeSymbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

void ShapeSymbol::loadResources()
{
	clear();

 	ShapeFileAdapter adapter;
 	adapter.load(m_filepath.c_str());
 
 	for (size_t i = 0, n = adapter.m_layers.size(); i < n; ++i)
 	{
 		for (size_t j = 0, m = adapter.m_layers[i]->polys.size(); j < m; ++j)
 		{
 			adapter.m_layers[i]->polys[j]->retain();
 			shapes.push_back(adapter.m_layers[i]->polys[j]);
 		}
 	}
}

void ShapeSymbol::clear()
{
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
		shapes[i]->release();
	shapes.clear();
}

} // d2d
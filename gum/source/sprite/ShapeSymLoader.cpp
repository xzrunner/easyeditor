#include "ShapeSymLoader.h"
#include "FilepathHelper.h"
#include "ShapeType.h"
#include "SymFileType.h"
#include "SymbolFactory.h"
#include "ImageSymbol.h"
#include "Image.h"

#include <sprite2/ShapeSymbol.h>
#include <sprite2/Color.h>
#include <sprite2/PolygonShape.h>
#include <sprite2/ColorPolygon.h>
#include <sprite2/TexturePolygon.h>
#include <simp/NodeShape.h>
#include <simp/from_int.h>

#include <fstream>

#include <assert.h>

namespace gum
{

ShapeSymLoader::ShapeSymLoader(s2::ShapeSymbol* sym)
	: m_sym(sym)
{
	if (m_sym) {
		m_sym->AddReference();
	}
}

ShapeSymLoader::~ShapeSymLoader()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void ShapeSymLoader::LoadJson(const std::string& filepath)
{
	if (!m_sym) {
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = FilepathHelper::Dir(filepath);

	// todo
}

void ShapeSymLoader::LoadBin(const simp::NodeShape* node)
{
	if (!m_sym) {
		return;
	}

	s2::Color col(node->color);
	std::vector<sm::vec2> vertices;
	vertices.reserve(node->vertices_n);
	int idx = 0;
	for (int i = 0; i < node->vertices_n; ++i) {
		float x = simp::int2float16x(int16_t(node->vertices[idx++])),
			  y = simp::int2float16x(int16_t(node->vertices[idx++]));
		vertices.push_back(sm::vec2(x, y));
	}

	switch (node->type)
	{
	case gum::SHAPE_POLYGON_COLOR:
		{
			s2::PolygonShape* polygon = new s2::PolygonShape(vertices);
			s2::Polygon* poly = new s2::ColorPolygon(col);
			poly->SetOutline(vertices);
			poly->Build();
			polygon->SetPolygon(poly);
			m_sym->SetShape(polygon);
		}
		break;
	case gum::SHAPE_POLYGON_TEXTURE:
		{
			s2::PolygonShape* polygon = new s2::PolygonShape(vertices);

			SymFileType type;
			s2::Symbol* sym = SymbolFactory::Instance()->Create(node->color, &type);
			assert(type == IMAGE);
			gum::ImageSymbol* tex_sym = VI_DOWNCASTING<gum::ImageSymbol*>(sym);
			s2::Polygon* poly = new s2::TexturePolygon(tex_sym);
			tex_sym->RemoveReference();

			poly->SetOutline(vertices);
			poly->Build();
			polygon->SetPolygon(poly);
			m_sym->SetShape(polygon);
		}
		break;
	}
}

}
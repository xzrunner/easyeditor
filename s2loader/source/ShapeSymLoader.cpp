#include "s2loader/ShapeSymLoader.h"
#include "s2loader/ShapeType.h"
#include "s2loader/ArrayLoader.h"

#include <sprite2/ShapeSymbol.h>
#include <painting2/Color.h>
#include <sprite2/PolygonShape.h>
#include <sprite2/ColorPolygon.h>
#include <sprite2/TexturePolygon.h>
#include <sprite2/SymType.h>
#include <simp/NodeShape.h>
#include <gum/FilepathHelper.h>
#include <gum/Image.h>
#include <gum/SymbolPool.h>
#include <gum/ImageSymbol.h>

#include <fstream>

#include <assert.h>

namespace s2loader
{

ShapeSymLoader::ShapeSymLoader(s2::ShapeSymbol& sym)
	: m_sym(sym)
{
}

void ShapeSymLoader::LoadJson(const CU_STR& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	CU_STR dir = gum::FilepathHelper::Dir(filepath);

	// todo
}

void ShapeSymLoader::LoadBin(const simp::NodeShape* node)
{
	pt2::Color col;
	col.FromRGBA(node->color);

	CU_VEC<sm::vec2> vertices;
	ArrayLoader::Load(vertices, node->vertices, node->vertices_n);

	switch (node->type)
	{
	case s2loader::SHAPE_POLYGON_COLOR:
		{
			auto poly_shape = std::make_unique<s2::PolygonShape>(vertices);
			auto poly = std::make_unique<s2::ColorPolygon>(col);
			poly->SetOutline(vertices);
			poly->Build();
			poly_shape->SetPolygon(std::move(poly));
			m_sym.SetShape(std::move(poly_shape));
		}
		break;
	case s2loader::SHAPE_POLYGON_TEXTURE:
		{
			auto poly_shape = std::make_unique<s2::PolygonShape>(vertices);

			auto sym = gum::SymbolPool::Instance()->Fetch(node->color);
			if (!sym) {
				return;
			}
			assert(sym->Type() == s2::SYM_IMAGE);
			auto tex_sym = S2_VI_PTR_DOWN_CAST<gum::ImageSymbol>(sym);
			auto poly = std::make_unique<s2::TexturePolygon>(tex_sym);

			poly->SetOutline(vertices);
			poly->Build();
			poly_shape->SetPolygon(std::move(poly));
			m_sym.SetShape(std::move(poly_shape));
		}
		break;
	}
}

}
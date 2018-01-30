#include "s2loader/TextureSymLoader.h"
#include "s2loader/ShapeLoader.h"
#include "s2loader/SymbolFactory.h"

#include <sprite2/TextureSymbol.h>
#include <sprite2/PolygonShape.h>
#include <sprite2/ShapeSymbol.h>
#include <simp/NodeTexture.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace s2loader
{

TextureSymLoader::TextureSymLoader(s2::TextureSymbol& sym)
	: m_sym(sym)
{
}

void TextureSymLoader::LoadJson(const CU_STR& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	if (!value.isMember("shapes")) {
		return;
	}

	CU_STR dir = gum::FilepathHelper::Dir(filepath);

	for (int i = 0, n = value["shapes"].size(); i < n; ++i) {
		auto shape = ShapeLoader::LoadShape(value["shapes"][i], dir);
#ifndef S2_SHAPE_SHARED_PTR
		auto poly_shape = S2_VI_DOWN_CAST<s2::PolygonShape*>(std::move(shape).release());
		auto poly_shape_ptr = std::unique_ptr<s2::PolygonShape>(poly_shape);
#else
		auto poly_shape = dynamic_cast<s2::PolygonShape*>(std::move(shape).release());
		auto poly_shape_ptr = std::shared_ptr<s2::PolygonShape>(poly_shape);
#endif // S2_SHAPE_SHARED_PTR
		m_sym.AddPolygon(poly_shape_ptr);
	}
}

void TextureSymLoader::LoadBin(const simp::NodeTexture* node)
{
	for (uint32_t i = 0; i < node->n; ++i) 
	{
		auto sym = SymbolFactory::Instance()->Create(node->polys[i]);
		if (sym)
		{
			auto shape_sym = S2_VI_PTR_DOWN_CAST<s2::ShapeSymbol>(sym);
#ifndef S2_SHAPE_SHARED_PTR
			auto poly_shape = S2_VI_DOWN_CAST<s2::PolygonShape*>(std::move(shape_sym->GetShape()).release());
			auto poly_shape_ptr = std::unique_ptr<s2::PolygonShape>(poly_shape);
#else
			auto poly_shape_ptr = std::dynamic_pointer_cast<s2::PolygonShape>(shape_sym->GetShape());
#endif // S2_SHAPE_SHARED_PTR
			m_sym.AddPolygon(poly_shape_ptr);
		}
	}
}

}
#include "Sprite.h"
#include "Symbol.h"
#include "ShapeFactory.h"

#include <ee/Shape.h>
#include <ee/SymbolType.h>

#include <gum/ShapeLoader.h>
#include <gum/ShapeSaver.h>

#include <assert.h>

namespace eshape
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::ShapeSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::ShapeSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::ShapeSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	if (val["shape"].isNull()) {
		return;
	}

	auto sym = std::dynamic_pointer_cast<Symbol>(m_sym);
	auto shape = gum::ShapeLoader::LoadShape(val["shape"], dir.c_str());
	sym->SetShape(std::move(shape));

	UpdateBounding();
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	auto sym = std::dynamic_pointer_cast<ee::Symbol>(m_sym);
	if (sym->GetFilepath().empty())
	{
		val["filepath"] = ee::SYM_SHAPE_TAG;

		Json::Value shape_val;

		auto sym = std::dynamic_pointer_cast<s2::ShapeSymbol>(m_sym);
		assert(sym);
		gum::ShapeSaver::Store(sym->GetShape(), shape_val);

		val["shape"] = shape_val;
	}
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}
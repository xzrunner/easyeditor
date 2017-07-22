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

Sprite::Sprite(Symbol* sym)
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

	Symbol* sym = VI_DOWNCASTING<Symbol*>(m_sym);
	s2::Shape* shape = gum::ShapeLoader::LoadShape(val["shape"], dir);
	sym->SetShape(shape);
	shape->RemoveReference();

	UpdateBounding();
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	ee::Symbol* sym = VI_DOWNCASTING<ee::Symbol*>(m_sym);
	if (sym->GetFilepath().empty())
	{
		val["filepath"] = ee::SYM_SHAPE_TAG;

		Json::Value shape_val;

		s2::ShapeSymbol* sym = VI_DOWNCASTING<s2::ShapeSymbol*>(m_sym);
		assert(sym);
		const s2::Shape* shape = VI_DOWNCASTING<const s2::Shape*>(sym->GetShape());
		gum::ShapeSaver::Store(shape, shape_val);

		val["shape"] = shape_val;
	}
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}
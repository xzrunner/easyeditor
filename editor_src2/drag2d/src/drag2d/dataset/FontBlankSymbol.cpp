#include "FontBlankSymbol.h"
#include "FontSprite.h"

#include "common/Vector.h"
#include "render/PrimitiveDraw.h"

#include <json/json.h>

namespace d2d
{

FontBlankSymbol::FontBlankSymbol()
{
	static int id = 0;
	m_name = wxT("fontblank") + wxVariant(id++);

	align = size = 0;
	width = height = 100;
}

FontBlankSymbol::~FontBlankSymbol()
{
}

void FontBlankSymbol::loadFromTextFile(std::ifstream& fin)
{
}

void FontBlankSymbol::storeToTextFile(std::ofstream& fout) const
{
}

void FontBlankSymbol::reloadTexture() const
{
}

void FontBlankSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
	float w = width, h = height;
	const FontSprite* s = dynamic_cast<const FontSprite*>(sprite);
	if (s)
	{
		w = s->width;
		h = s->height;
	}
	PrimitiveDraw::drawRect(Vector(0, 0), w*0.5f, h*0.5f, true, 2, Colorf(0.7f, 0.7f, 0.7f, 0.7f));
}

float FontBlankSymbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return width;
}

float FontBlankSymbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return height;
}

void FontBlankSymbol::refresh()
{
}

void FontBlankSymbol::loadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(m_filepath.fn_str());
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();
	font = value["font"].asString();
	color = value["color"].asString();
	align = value["align"].asDouble();
	size = value["size"].asDouble();
	width = value["width"].asDouble();
	height = value["height"].asDouble();
}
} // d2d
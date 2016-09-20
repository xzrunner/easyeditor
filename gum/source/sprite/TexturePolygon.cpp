#include "TexturePolygon.h"
#include "SymFileType.h"
#include "SymbolFactory.h"

#include <sprite2/ImageSymbol.h>

#include <assert.h>

namespace gum
{

TexturePolygon::TexturePolygon(const std::string& filepath)
{
	SymFileType type;
	s2::Symbol* sym = SymbolFactory::Instance()->Create(filepath, &type);
	assert(type == IMAGE);

	m_img = dynamic_cast<s2::ImageSymbol*>(sym);
}

}
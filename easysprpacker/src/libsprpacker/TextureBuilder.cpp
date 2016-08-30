#include "TextureBuilder.h"
#include "PackTexture.h"

#include <easytexture.h>

namespace esprpacker
{

SINGLETON_DEFINITION(TextureBuilder);

TextureBuilder::TextureBuilder()
{
}

TextureBuilder::~TextureBuilder()
{
	Clear();
}

void TextureBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::map<const etexture::Symbol*, const PackTexture*>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackTexture*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void TextureBuilder::Clear()
{
	std::map<const etexture::Symbol*, const PackTexture*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* TextureBuilder::Create(const etexture::Symbol* sym)
{
	std::map<const etexture::Symbol*, const PackTexture*>::iterator
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	PackTexture* node = new PackTexture(sym);
	sym->AddReference();
	m_map_data.insert(std::make_pair(sym, node));
	node->AddReference();
	return node;
}

}
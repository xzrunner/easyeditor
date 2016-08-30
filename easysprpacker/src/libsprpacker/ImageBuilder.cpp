#include "ImageBuilder.h"
#include "PackImage.h"

#include <ee/ImageSymbol.h>

namespace esprpacker
{

SINGLETON_DEFINITION(ImageBuilder);

ImageBuilder::ImageBuilder()
{
}

ImageBuilder::~ImageBuilder()
{
	Clear();
}

void ImageBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::map<const ee::ImageSymbol*, const PackImage*>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackImage*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void ImageBuilder::Clear()
{
	std::map<const ee::ImageSymbol*, const PackImage*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* ImageBuilder::Create(const ee::ImageSymbol* sym)
{
	std::map<const ee::ImageSymbol*, const PackImage*>::iterator
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	PackImage* node = new PackImage(sym);
	sym->AddReference();
	m_map_data.insert(std::make_pair(sym, node));
	node->AddReference();
	return node;
}

}
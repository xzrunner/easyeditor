#include "ClipboxBuilder.h"
#include "PackClipbox.h"
#include "Utility.h"

#include <ee/Visitor.h>
#include <ee/Exception.h>

#include <easycomplex.h>

namespace erespacker
{

ClipboxBuilder::ClipboxBuilder()
{
}

ClipboxBuilder::~ClipboxBuilder()
{
	std::map<const ecomplex::Symbol*, const PackClipbox*>::iterator itr 
		= m_map_clipbox.begin();
	for ( ; itr != m_map_clipbox.end(); ++itr) {
		delete itr->second;
	}
}

void ClipboxBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	std::map<const ecomplex::Symbol*, const PackClipbox*>::const_iterator itr 
		= m_map_clipbox.begin();
	for ( ; itr != m_map_clipbox.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackClipbox*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* ClipboxBuilder::Create(const ecomplex::Symbol* complex)
{
	std::map<const ecomplex::Symbol*, const PackClipbox*>::iterator itr 
		= m_map_clipbox.find(complex);
	if (itr != m_map_clipbox.end()) {
		return itr->second;
	}

	const sm::rect& scissor = complex->GetScissor();
	if (scissor.xmin == 0 &&
		scissor.ymin == 0 &&
		scissor.xmax == 0 &&
		scissor.ymax == 0) {
		return NULL;
	}

	PackClipbox* cb = new PackClipbox;
	cb->x = scissor.xmin;
	cb->y = scissor.ymax;
	sm::vec2 sz = scissor.Size();
	cb->w = sz.x;
	cb->h = sz.y;
	m_map_clipbox.insert(std::make_pair(complex, cb));

	return cb;
}

}
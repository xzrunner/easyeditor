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

void ClipboxBuilder::Traverse(ee::Visitor& visitor) const
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

	if (complex->m_clipbox.xmin == 0 &&
		complex->m_clipbox.ymin == 0 &&
		complex->m_clipbox.xmax == 0 &&
		complex->m_clipbox.ymax == 0) {
		return NULL;
	}

	const sm::rect& r = complex->m_clipbox;

	PackClipbox* cb = new PackClipbox;
	cb->x = r.xmin;
	cb->y = r.ymax;
	sm::vec2 sz = r.Size();
	cb->w = sz.x;
	cb->h = sz.y;
	m_map_clipbox.insert(std::make_pair(complex, cb));

	return cb;
}

}
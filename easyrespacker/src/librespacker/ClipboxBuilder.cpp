#include "ClipboxBuilder.h"
#include "PackClipbox.h"
#include "Utility.h"

namespace librespacker
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

void ClipboxBuilder::Traverse(d2d::IVisitor& visitor) const
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

	if (complex->m_clipbox.xMin == 0 &&
		complex->m_clipbox.yMin == 0 &&
		complex->m_clipbox.xMax == 0 &&
		complex->m_clipbox.yMax == 0) {
		return NULL;
	}

	const d2d::Rect& r = complex->m_clipbox;

	PackClipbox* cb = new PackClipbox;
	cb->x = r.xMin;
	cb->y = r.yMax;
	cb->w = r.xLength();
	cb->h = r.yLength();
	m_map_clipbox.insert(std::make_pair(complex, cb));

	return cb;
}

}
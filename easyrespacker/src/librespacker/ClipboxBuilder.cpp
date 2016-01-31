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

	const ee::Rect& r = complex->m_clipbox;

	PackClipbox* cb = new PackClipbox;
	cb->x = r.xmin;
	cb->y = r.ymax;
	cb->w = r.Width();
	cb->h = r.Height();
	m_map_clipbox.insert(std::make_pair(complex, cb));

	return cb;
}

}
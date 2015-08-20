#include "Picture.h"
#include "type.h"

namespace epbin
{
namespace epe
{

Picture::Picture(lua_State* L)
	: INode(TYPE_PICTURE)
{
	Load(L);
}

size_t Picture::Size() const
{
	size_t sz = INode::Size();
	sz += sizeof(uint16_t);		// size
	sz += sizeof(Quad) * m_quads.size();
	return sz;
}

void Picture::Store(uint8_t** ptr)
{
	INode::Store(ptr);

	uint16_t sz = m_quads.size();
	memcpy(*ptr, &sz, sizeof(sz));
	*ptr += sizeof(sz);

	for (int i = 0, n = m_quads.size(); i < n; ++i) {
		const Quad& q = m_quads[i];
		
		memcpy(*ptr, &q.texid, sizeof(q.texid));
		*ptr += sizeof(q.texid);

		for (int i = 0; i < 8; ++i) {
			memcpy(*ptr, &q.texture_coord[i], sizeof(q.texture_coord[i]));
			*ptr += sizeof(q.texture_coord[i]);
		}

		for (int i = 0; i < 8; ++i) {
			memcpy(*ptr, &q.screen_coord[i], sizeof(q.screen_coord[i]));
			*ptr += sizeof(q.screen_coord[i]);
		}
	}
}

void Picture::Load(lua_State* L)
{
	
}

}
}
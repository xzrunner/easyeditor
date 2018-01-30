#include "s2loader/ResourceUID.h"
#include "s2loader/GlyphStyle.h"
#include "s2loader/GlyphStyleID.h"

namespace s2loader
{

ResourceUID::Type ResourceUID::GetType(UID uid)
{
	return static_cast<ResourceUID::Type>((uid & TYPE_MASK) >> DATA_SIZE);
}

UID ResourceUID::RawFile(const CU_STR& filepath)
{
	return Compose(HashString(filepath), RES_RAW_FILE);
}

UID ResourceUID::Glyph(int unicode, const GlyphStyle& gs)
{
	uint64_t style_id = GlyphStyleID::Instance()->Gen(gs);
	uint64_t id = ((style_id & 0xffffffff) << 32) | unicode;
	return Compose(id, RES_GLYPH);
}

UID ResourceUID::BinNode(uint32_t node_id)
{
	return Compose(node_id, RES_BIN_NODE);
}

UID ResourceUID::Sprite(int spr_id)
{
	return Compose(spr_id, RES_SPRITE);
}

UID ResourceUID::Actor(const s2::Actor* actor)
{
	uint64_t actor_id = reinterpret_cast<uint64_t>(actor);
	return Compose(actor_id, RES_ACTOR);
}

uint64_t ResourceUID::HashString(const CU_STR& str)
{
	const char* c = str.c_str();

	// BKDR Hash Function
	uint64_t seed = 131; // 31 131 1313 13131 131313 etc..
	uint64_t hash = 0;

	while (*c) {
		hash = hash * seed + (*c++);
	}

	return hash;
}

UID ResourceUID::Compose(uint64_t data, Type type)
{
	return (data & DATA_MASK) | ((uint64_t)type << DATA_SIZE);
}

}
#ifndef _EPBIN_EPE_PACK_ANIMATION_H_
#define _EPBIN_EPE_PACK_ANIMATION_H_

#include "../common_dataset.h"
#include "../IPackNode.h"

#include <vector>

struct lua_State;

namespace epbin
{
namespace epe
{

class PackAnimation : public IPackNode
{
public:
	PackAnimation(lua_State* L, uint16_t id);
	virtual ~PackAnimation();

	virtual size_t Size() const;
	virtual void Store(std::ofstream& fout) const;

private:
	struct Clipbox
	{
		int32_t cb[4];
		size_t Size() const { return sizeof(cb); }
	};

	struct Component
	{
		uint16_t id;
		String name;
	};

	struct Action
	{
		String name;
		uint16_t size;
	};

	struct Matrix
	{
		int m[6];
	};

	struct SpriteTrans 
	{
		int matref;
		Matrix mat;

		uint32_t color;
		uint32_t additive;
		uint32_t rmap, gmap, bmap;

		int program;
	};

	struct PackPart
	{
		bool only_number;

		uint8_t type;

		SpriteTrans t;
		uint16_t component_id;
		int8_t touchable;
	};

	struct PackFrame 
	{
		std::vector<PackPart> parts;
	};

private:
	void Load(lua_State* L);
	void LoadFrame(lua_State* L, PackFrame& frame);

	size_t FrameSize(const PackFrame& frame) const;
	void StoreFrame(const PackFrame& frame, std::ofstream& fout) const;

	static bool IsMatrixIdentity(lua_State* L);

private:
//	String m_export_name;

	Clipbox* m_clipbox;

	std::vector<Component> m_components;

	std::vector<Action> m_actions;	

	std::vector<PackFrame> m_frames;

}; // PackAnimation

}
}

#endif // _EPBIN_EPE_PACK_ANIMATION_H_
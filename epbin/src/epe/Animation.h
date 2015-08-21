#ifndef _EPBIN_EPE_ANIMATION_H_
#define _EPBIN_EPE_ANIMATION_H_

#include "../common_dataset.h"
#include "../INode.h"

#include <vector>

struct lua_State;

namespace epbin
{
namespace epe
{

class Animation : public INode
{
public:
	Animation(lua_State* L);
	virtual ~Animation();

	virtual size_t Size() const;

	virtual void Store(uint8_t** ptr);

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
		Matrix* mat;
		uint32_t color;
		uint32_t additive;
		uint32_t rmap, gmap, bmap;
		int program;
	};

	struct PackPart
	{
		SpriteTrans t;
		int component_id;
		int touchable;
	};

	struct PackFrame 
	{
		std::vector<PackPart> parts;
	};

private:
	void Load(lua_State* L);
	void LoadFrame(lua_State* L);

private:
	String m_export_name;

	Clipbox* m_clipbox;

	std::vector<Component> m_components;

	std::vector<Action> m_actions;	

	std::vector<PackFrame> m_frames;

}; // Animation

}
}

#endif // _EPBIN_EPE_ANIMATION_H_
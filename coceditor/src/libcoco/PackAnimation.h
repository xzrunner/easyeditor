#ifndef _LIBCOCO_PACK_ANIMATION_H_
#define _LIBCOCO_PACK_ANIMATION_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace ebuilder { class CodeGenerator; }

namespace libcoco
{

class PackAnimation : public IPackNode
{
public:
	// component

	struct Component
	{
		const IPackNode* node;
		std::string name;
	};

	// action

	struct Action
	{
		std::string name;
		int size;

		Action() : size(0) {}
	};

	// frame

	struct SpriteTrans
	{
		int mat[6];

		uint32_t color;
		uint32_t additive;
		uint32_t rmap, gmap, bmap;

		int program;

		SpriteTrans() 
			: color(0xffffffff)
			, additive(0)
			, rmap(0xff0000ff)
			, gmap(0x00ff00ff) 
			, bmap(0x0000ffff) {
			mat[1] = mat[2] = mat[4] = mat[5] = 0;
			mat[0] = mat[3] = 1024;
		}
	};

	struct Part
	{
		SpriteTrans t;
		int comp_idx;
		bool touchable;

		Part() : comp_idx(-1), touchable(true) {}
	};

	struct Frame
	{
		std::vector<Part> parts;
	};

public:
	PackAnimation() {}
	PackAnimation(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const d2d::TexturePacker& tp) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<d2d::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const d2d::TexturePacker& tp) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<d2d::Image*>& images);

	void CreateFramePart(const d2d::ISprite* spr, Frame& frame);

	void Clear();

	static bool IsMatrixIdentity(const int* mat);

private:
	int AddComponent(const IPackNode* node, const std::string& name);	

	static void LoadSprTrans(const d2d::ISprite* spr, SpriteTrans& trans);
	static void LoadSprMat(const d2d::ISprite* spr, SpriteTrans& trans);
	static void LoadSprColor(const d2d::ISprite* spr, SpriteTrans& trans);

public:
	std::vector<Component> components;

	std::vector<Action> actions;

	std::vector<Frame> frames;

}; // PackAnimation

}

#endif // _LIBCOCO_PACK_ANIMATION_H_
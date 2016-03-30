#ifndef _EASYRESPACKER_PACK_ANIMATION_H_
#define _EASYRESPACKER_PACK_ANIMATION_H_

#include "IPackNode.h"

#include <float.h>

namespace ebuilder { class CodeGenerator; }
namespace ee { class Sprite; }

namespace erespacker
{

class PackClipbox;

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

		uint16_t blend;
		uint16_t filter;

		uint16_t camera;

		SpriteTrans() 
			: color(0xffffffff)
			, additive(0)
			, rmap(0xff0000ff)
			, gmap(0x00ff00ff) 
			, bmap(0x0000ffff)
			, blend(0)
			, filter(0)
			, camera(0) {
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
		const ee::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::Image*>& images);

	void CreateFramePart(const ee::Sprite* spr, Frame& frame);
	void CreateClipboxFramePart(const PackClipbox* cb, Frame& frame);

	void Clear();

	static bool IsMatrixIdentity(const int* mat);

private:
	bool AddComponent(const IPackNode* node, const std::string& name, int& comp_idx);	

	static void LoadSprTrans(const ee::Sprite* spr, SpriteTrans& trans, bool force_mat);
	static void LoadSprMat(const ee::Sprite* spr, SpriteTrans& trans, bool force);
	static void LoadSprColor(const ee::Sprite* spr, SpriteTrans& trans);

public:
	std::vector<Component> components;

	std::vector<Action> actions;

	std::vector<Frame> frames;

}; // PackAnimation

}

#endif // _EASYRESPACKER_PACK_ANIMATION_H_
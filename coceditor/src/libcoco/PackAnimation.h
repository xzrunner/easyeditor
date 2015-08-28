#ifndef _LIBCOCO_PACK_ANIMATION_H_
#define _LIBCOCO_PACK_ANIMATION_H_

#include "IPackNode.h"

#include <drag2d.h>

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
			, rmap(0xffff0000)
			, gmap(0xff00ff00) 
			, bmap(0xff0000ff) {
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
	virtual std::string ToString() const;

	void CreateFramePart(const d2d::ISprite* spr, Frame& frame);

private:
	int AddComponent(const IPackNode* node, const std::string& name);	

	static void LoadSprTrans(const d2d::ISprite* spr, SpriteTrans& trans);

public:
	std::string export_name;

	std::vector<Component> components;

	std::vector<Action> actions;

	std::vector<Frame> frames;

}; // PackAnimation

}

#endif // _LIBCOCO_PACK_ANIMATION_H_
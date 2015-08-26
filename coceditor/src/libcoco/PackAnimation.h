#ifndef _LIBCOCO_PACK_ANIMATION_H_
#define _LIBCOCO_PACK_ANIMATION_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace ecomplex { class Symbol; }
namespace libanim { class Symbol; }

namespace libcoco
{

class PackAnimation : public IPackNode
{
public:
	PackAnimation(const ecomplex::Symbol* complex);
	PackAnimation(const libanim::Symbol* anim);
	
private:
	void LoadComplex(const ecomplex::Symbol* complex);
	void LoadAnim(const libanim::Symbol* anim);

private:
	struct Action
	{
		std::string name;
		int size;

		Action() : size(0) {}
	};

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
			memset(mat, 0, sizeof(mat));	
		}
	};

	struct Frame
	{
		SpriteTrans t;
		int comp_idx;
		bool touchable;

		Frame() : comp_idx(-1), touchable(true) {}
	};

private:
	std::vector<IPackNode*> m_components;

	std::vector<Action> m_actions;

	std::vector<Frame> m_frames;

}; // PackAnimation

}

#endif // _LIBCOCO_PACK_ANIMATION_H_
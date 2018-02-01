#ifndef _S2LOADER_TRAIL_SYM_LOADER_H_
#define _S2LOADER_TRAIL_SYM_LOADER_H_

#include <cu/uncopyable.h>
#include <painting2/Color.h>
#include <sprite2/typedef.h>
#include <cu/cu_stl.h>

#include <json/json.h>

struct t2d_emitter_cfg;

namespace s2 { class Symbol; class TrailSymbol; }
namespace simp { class NodeTrail; }

namespace s2loader
{

class TrailSymLoader : private cu::Uncopyable
{
public:
	TrailSymLoader();

	void Store(const std::shared_ptr<s2::TrailSymbol>& sym) const;
	void Store(t2d_emitter_cfg* cfg) const;

	void LoadJson(const CU_STR& filepath);
	void LoadBin(const simp::NodeTrail* node);

private:
	void LoadImageComp(const CU_STR& dir, const Json::Value& comp_val);
	void LoadShapeComp(const Json::Value& comp_val);

protected:
	virtual s2::SymPtr LoadSymbol(const CU_STR& filepath) const;

public:
	struct CompImage
	{
		uint32_t sym_id;
		CU_STR filepath;
		float scale_begin, scale_end;
		pt2::Color mul_col_begin, mul_col_end;
		pt2::Color add_col_begin, add_col_end;

		CompImage()
			: scale_begin(0)
			, scale_end(0)
		{}

	}; // CompImage

	struct CompShape
	{
		float linewidth;
		float acuity;
		pt2::Color col_begin, col_end;

		CompShape()
			: linewidth(1)
			, acuity(1)
		{}

	}; // CompShape

public:
	int mode;

	int count;

	float life_begin;
	float life_offset;

	float fadeout_time;

	CU_VEC<CompImage> comp_images;
	CU_VEC<CompShape> comp_shapes;

}; // TrailSymLoader

}

#endif // _S2LOADER_TRAIL_SYM_LOADER_H_

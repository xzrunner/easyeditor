#ifndef _GUM_TRAIL_SYM_LOADER_H_
#define _GUM_TRAIL_SYM_LOADER_H_

#include <CU_Uncopyable.h>

#include <sprite2/Color.h>

#include <json/json.h>

#include <string>

struct t2d_emitter_cfg;

namespace s2 { class Symbol; class TrailSymbol; }
namespace simp { class NodeTrail; }

namespace gum
{

class TrailSymLoader : private cu::Uncopyable
{
public:
	void Store(s2::TrailSymbol* sym) const;
	void Store(t2d_emitter_cfg* cfg) const;

	void LoadJson(const std::string& filepath);
	void LoadBin(const simp::NodeTrail* node);

private:
	void LoadImageComp(const std::string& dir, const Json::Value& comp_val);
	void LoadShapeComp(const Json::Value& comp_val);

protected:
	virtual s2::Symbol* LoadSymbol(const std::string& filepath) const;

public:
	struct CompImage
	{
		uint32_t sym_id;
		std::string filepath;
		float scale_begin, scale_end;
		s2::Color mul_col_begin, mul_col_end;
		s2::Color add_col_begin, add_col_end;

		CompImage()
			: scale_begin(0)
			, scale_end(0)
		{}

	}; // CompImage

	struct CompShape
	{
		float linewidth;
		float acuity;
		s2::Color col_begin, col_end;

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

	std::vector<CompImage> comp_images;
	std::vector<CompShape> comp_shapes;

}; // TrailSymLoader

}

#endif // _GUM_TRAIL_SYM_LOADER_H_

#ifndef _S2LOADER_P2D_SYM_LOADER_H_
#define _S2LOADER_P2D_SYM_LOADER_H_

#include <cu/uncopyable.h>
#include <SM_Vector.h>
#include <painting2/Color.h>
#include <sprite2/typedef.h>
#include <cu/cu_stl.h>

#include <json/json.h>

#include <memory>

namespace s2 { class Symbol; class Particle2dSymbol; }
namespace simp { class NodeParticle2d; }

struct p2d_emitter_cfg;

namespace s2loader
{

class P2dSymLoader : private cu::Uncopyable
{
public:
	P2dSymLoader();

	void Store(const std::shared_ptr<s2::Particle2dSymbol>& sym) const;
	void Store(p2d_emitter_cfg* cfg) const;

	void LoadJson(const CU_STR& filepath);	
	void LoadBin(const simp::NodeParticle2d* node);

private:
	void LoadComponent(const CU_STR& dir, const Json::Value& comp_val);

protected:
	virtual s2::SymPtr LoadSymbol(const CU_STR& filepath) const;

public:
	struct Component
	{
		float angle_start, angle_end;

		float scale_start, scale_end;

		pt2::Color mul_col_begin, mul_col_end;
		pt2::Color add_col_begin, add_col_end;

		float alpha_start, alpha_end;

		uint32_t sym_id;
		CU_STR filepath;

		Component()
			: angle_start(0)
			, angle_end(0)
			, scale_start(0)
			, scale_end(0)
			, alpha_start(0)
			, alpha_end(0)
			, sym_id(0)
		{
		}

	}; // Component

public:
	CU_STR name;

	int mode_type;

	//////////////////////////////////////////////////////////////////////////
	// A: gravity + tangential + radial
	//////////////////////////////////////////////////////////////////////////
	struct A
	{
		sm::vec2 gravity;

		float speed, speed_var;

		float tangential_accel, tangential_accel_var;
		float radial_accel, radial_accel_var;

		bool rotation_is_dir;
	};

	//////////////////////////////////////////////////////////////////////////
	// B: radius + rotate
	//////////////////////////////////////////////////////////////////////////
	struct B
	{
		float start_radius, start_radius_var;
		float end_radius, end_radius_var;

		float direction_delta, direction_delta_var;
	};

	//////////////////////////////////////////////////////////////////////////
	// C: tangential spd cos
	//////////////////////////////////////////////////////////////////////////
	struct C
	{
		float speed, speed_var;

		float cos_amplitude, cos_amplitude_var;
		float cos_frequency, cos_frequency_var;
	};

	A A;
	B B;
	C C;

	float emission_time;
	int count;

	float life, life_var;

	sm::vec2 position, position_var;

	float direction, direction_var;

	CU_VEC<Component> components;

}; // P2dSymLoader

}

#endif // _S2LOADER_P2D_SYM_LOADER_H_

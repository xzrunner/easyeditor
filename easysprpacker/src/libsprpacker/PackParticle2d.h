#ifndef _EASYSPRPACKER_PACK_PARTICLE2D_H_
#define _EASYSPRPACKER_PACK_PARTICLE2D_H_

#include "PackNode.h"

#include <sprite2/Particle2dSymbol.h>

#include <ps_2d.h>

#include <vector>

namespace eparticle2d { class Symbol; }

namespace esprpacker
{

class PackParticle2d : public PackNode
{
public:
	PackParticle2d(const eparticle2d::Symbol* sym);

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, 
		float scale) const;	
	
private:
	void Init(const eparticle2d::Symbol* sym);	

	void PackToLuaPS(ebuilder::CodeGenerator& gen) const;

private:
	class Component
	{
	public:
		Component(const p2d_symbol& sym);
		~Component();

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;

		int SizeOfUnpackFromBin() const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;

	public:
		float m_angle_start, m_angle_end;

		float m_scale_start, m_scale_end;

		uint32_t m_mul_col_begin, m_mul_col_end;
		uint32_t m_add_col_begin, m_add_col_end;

		const PackNode* m_node;

	}; // Component

private:
	float m_emission_time;
	int m_count;

	float m_life, m_life_var;

	sm::vec2 m_position, m_position_var;

	float m_direction, m_direction_var;

	std::vector<Component> m_components;

	int m_mode_type;

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

}; // PackParticle2d

}

#endif // _EASYSPRPACKER_PACK_PARTICLE2D_H_
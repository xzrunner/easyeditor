#ifndef _EASYSPRPACKER_PACK_TRAIL_H_
#define _EASYSPRPACKER_PACK_TRAIL_H_

#include "PackNode.h"

#include <sprite2/TrailSymbol.h>

#include <mt_2d.h>

#include <vector>

namespace etrail { class Symbol; }

namespace esprpacker
{

class PackTrail : public PackNode
{
public:
	PackTrail(const etrail::Symbol* sym);

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
	void Init(const etrail::Symbol* sym);	
	
	void PackToLuaMT(ebuilder::CodeGenerator& gen) const;

private:
	class CompImage
	{
	public:
		CompImage(const t2d_symbol& sym);
		~CompImage();

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;

		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;

	public:
		const PackNode* m_node;
		float m_scale_begin, m_scale_end;
		uint32_t m_mul_col_begin, m_mul_col_end,
			     m_add_col_begin, m_add_col_end;

	}; // CompImage

	class CompShape
	{
	public:
		CompShape(const t2d_symbol& sym);

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;

		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;

	public:
		float m_linewidth;
		float m_acuity;
		uint32_t m_col_begin, m_col_end;

	}; // CompShape

private:
	int m_mode;

	int m_count;

	float m_life_begin;
	float m_life_offset;

	float m_fadeout_time;

	std::vector<CompImage> m_comp_images;
	std::vector<CompShape> m_comp_shapes;

}; // PackTrail

}

#endif // _EASYSPRPACKER_PACK_TRAIL_H_
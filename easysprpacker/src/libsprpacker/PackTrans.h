#ifndef _EASYSPRPACKER_PACK_TRANS_H_
#define _EASYSPRPACKER_PACK_TRANS_H_

#include <SM_Vector.h>
#include <cu/cu_stl.h>
#include <painting2/RenderColorCommon.h>
#include <painting2/RenderColorMap.h>
#include <painting2/FastBlendMode.h>
#include <painting2/BlendMode.h>
#include <painting2/RenderCamera.h>

#include <string>

namespace s2 { class Sprite; }
namespace ebuilder { class CodeGenerator; class RenderFilter;  }
namespace pt2 { class RenderFilter; }

namespace esprpacker
{

class PackTrans
{
public:
	PackTrans();
	PackTrans(const s2::Sprite& spr, bool force_name = false);
	~PackTrans();

	bool operator == (const PackTrans& trans) const;

	void PackToLua(ebuilder::CodeGenerator& gen) const;

	int SizeOfUnpackFromBin() const;
	int SizeOfPackToBin() const;
	void PackToBin(uint8_t** ptr) const;

	const std::string& GetName() const { return m_name; }

private:
	static int ToInt(float f) {
		return static_cast<int>(floor(f * 1024.0f + 0.5f));
	}

private:
	uint32_t			   m_type;

	// geometry
	sm::vec2			   m_scale;
	sm::vec2			   m_shear;
	sm::vec2			   m_offset;
	sm::vec2			   m_position;
	float				   m_angle;

	// shader
	pt2::RenderColorCommon m_col_common;
	pt2::RenderColorMap    m_col_map;
	pt2::BlendMode		   m_blend;
	pt2::FastBlendMode	   m_fast_blend;
	pt2::RenderFilter*	   m_filter;
	float                  m_downsample;
	pt2::RenderCamera	   m_camera;
	// other
	bool                   m_need_actor;
	bool                   m_integrate;

	// info
	std::string			   m_name;

	// edit
	bool				   m_visible;

}; // PackTrans

}

#endif // _EASYSPRPACKER_PACK_TRANS_H_
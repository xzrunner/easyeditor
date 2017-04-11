#ifndef _EASYSPRPACKER_PACK_TRANS_H_
#define _EASYSPRPACKER_PACK_TRANS_H_

#include <sprite2/RenderColor.h>
#include <sprite2/BlendMode.h>
#include <sprite2/FastBlendMode.h>
#include <sprite2/RenderCamera.h>

#include <SM_Vector.h>

#include <string>

namespace s2 { class Sprite; class RenderFilter; }
namespace ebuilder { class CodeGenerator; }

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
	uint32_t			m_type;

	// geometry
	sm::vec2			m_scale;
	sm::vec2			m_shear;
	sm::vec2			m_offset;
	sm::vec2			m_position;
	float				m_angle;

	// shader
	s2::RenderColor		m_color;
	s2::BlendMode		m_blend;
	s2::FastBlendMode	m_fast_blend;
	const s2::RenderFilter*	m_filter;
	s2::RenderCamera	m_camera;

	// actor
	bool                m_need_actor;

	// info
	std::string			m_name;

	// edit
	bool				m_visible;

}; // PackTrans

}

#endif // _EASYSPRPACKER_PACK_TRANS_H_
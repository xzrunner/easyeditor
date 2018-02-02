#include "PackTrans.h"
#include "binary_io.h"

#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <sprite2/Sprite.h>
#include <simp/NodeTrans.h>
#include <painting2/RenderShader.h>

namespace esprpacker
{

PackTrans::PackTrans()
	: m_type(0)
	, m_filter(NULL)
	, m_visible(true)
	, m_need_actor(false)
	, m_integrate(false)
{
}

PackTrans::PackTrans(const s2::Sprite& spr, bool force_name)
{
	m_type = 0;

	// geometry

	m_scale = spr.GetScale();
	if (m_scale != sm::vec2(1, 1)) {
		m_type |= simp::NodeTrans::SCALE_MASK;
	}

	m_shear = spr.GetShear();
	if (m_shear != sm::vec2(0, 0)) {
		m_type |= simp::NodeTrans::SHEAR_MASK;
	}

	m_offset = spr.GetOffset();
	if (m_offset != sm::vec2(0, 0)) {
		m_type |= simp::NodeTrans::OFFSET_MASK;
	}

	m_position = spr.GetPosition();
	if (m_position != sm::vec2(0, 0)) {
		m_type |= simp::NodeTrans::POSITION_MASK;
	}

	m_angle = spr.GetAngle();
	if (m_angle != 0) {
		m_type |= simp::NodeTrans::ANGLE_MASK;
	}

	// shader

	m_col_common = spr.GetColorCommon();
	if (m_col_common.mul.ToABGR() != 0xffffffff) {
		m_type |= simp::NodeTrans::COL_MUL_MASK;
	}
	if (m_col_common.add.ToABGR() != 0) {
		m_type |= simp::NodeTrans::COL_ADD_MASK;
	}

	m_col_map = spr.GetColorMap();
	if (m_col_map.rmap.ToABGR() != 0x000000ff) {
		m_type |= simp::NodeTrans::COL_R_MASK;
	}
	if (m_col_map.gmap.ToABGR() != 0x0000ff00) {
		m_type |= simp::NodeTrans::COL_G_MASK;
	}
	if (m_col_map.bmap.ToABGR() != 0x00ff0000) {
		m_type |= simp::NodeTrans::COL_B_MASK;
	}

	m_blend = spr.GetShader().GetBlend();
	if (m_blend != pt2::BM_NULL) {
		m_type |= simp::NodeTrans::BLEND_MASK;
	}

	m_fast_blend = spr.GetShader().GetFastBlend();
	if (m_fast_blend != pt2::FBM_NULL) {
		m_type |= simp::NodeTrans::FAST_BLEND_MASK;
	}

	m_filter = spr.GetShader().GetFilter().get();
	if (m_filter && m_filter->GetMode() != pt2::FM_NULL) {
		m_type |= simp::NodeTrans::FILTER_MASK;
	}

	m_downsample = spr.GetShader().GetDownsample();
	if (fabs(m_downsample - 1) > FLT_EPSILON) {
		m_type |= simp::NodeTrans::DOWNSMAPLE_MASK;
	}

	m_camera = spr.GetCamera();
	if (m_camera.GetMode() != pt2::CM_ORTHO) {
		m_type |= simp::NodeTrans::CAMERA_MASK;
	}

	// other
	m_need_actor = spr.IsNeedActor();
	if (m_need_actor) {
		m_type |= simp::NodeTrans::ACTOR_MASK;
	}
	m_integrate = spr.IsIntegrate();
	if (m_integrate) {
		m_type |= simp::NodeTrans::INTEGRATE_MASK;
	}

	// integrate

	CU_STR name;
	s2::SprNameMap::Instance()->IDToStr(spr.GetName(), name);
	if (!name.empty()) {
		if (name[0] == '_') {
			if (force_name) {
				if (name.compare(0, strlen("_sprite"), "_sprite") == 0) {
					m_name = std::string("_") + name.substr(strlen("_sprite")).c_str();
				} else {
					m_name = name.c_str();
				}
			}
		} else {
			m_name = name.c_str();
		}
	}

	m_visible = spr.IsVisible();
}

PackTrans::~PackTrans()
{
}

bool PackTrans::operator == (const PackTrans& trans) const
{
	if (!m_filter && !trans.m_filter) {
		;
	} else if (m_filter && trans.m_filter) {
		if (*m_filter == *trans.m_filter) {
			;
		} else {
			return false;
		}
	} else {
		return false;
	}

	return m_type == trans.m_type 
		&& m_scale == trans.m_scale 
		&& m_shear == trans.m_shear 
		&& m_offset == trans.m_offset 
		&& m_position == trans.m_position 
		&& m_angle == trans.m_angle 
		&& m_col_common == trans.m_col_common
		&& m_col_map == trans.m_col_map
		&& m_blend == trans.m_blend 
		&& m_fast_blend == trans.m_fast_blend 
		&& fabs(m_downsample - trans.m_downsample) < FLT_EPSILON
		&& m_camera == trans.m_camera 
		&& m_need_actor == trans.m_need_actor
		&& m_integrate == trans.m_integrate
		&& m_name == trans.m_name 
		&& m_visible == trans.m_visible;
}

void PackTrans::PackToLua(ebuilder::CodeGenerator& gen) const
{
	// geo
	std::vector<std::string> geos;
	if (m_type & simp::NodeTrans::SCALE_MASK) {
		geos.push_back(lua::assign("scale_x", m_scale.x));
		geos.push_back(lua::assign("scale_y", m_scale.y));
	}
	if (m_type & simp::NodeTrans::SHEAR_MASK) {
		geos.push_back(lua::assign("shear_x", m_shear.x));
		geos.push_back(lua::assign("shear_y", m_shear.y));
	}
	if (m_type & simp::NodeTrans::OFFSET_MASK) {
		geos.push_back(lua::assign("offset_x", m_offset.x));
		geos.push_back(lua::assign("offset_y", m_offset.y));
	}
	if (m_type & simp::NodeTrans::POSITION_MASK) {
		geos.push_back(lua::assign("pos_x", m_position.x));
		geos.push_back(lua::assign("pos_y", m_position.y));
	}
	if (m_type & simp::NodeTrans::ANGLE_MASK) {
		geos.push_back(lua::assign("angle", m_angle));		
	}
	lua::connect(gen, geos);

	// color
	std::vector<std::string> colors;
	if (m_type & simp::NodeTrans::COL_MUL_MASK) {
		colors.push_back(lua::assign("mul", ee::StringHelper::ToString(m_col_common.mul.ToRGBA())));
	}
	if (m_type & simp::NodeTrans::COL_ADD_MASK) {
		colors.push_back(lua::assign("add", ee::StringHelper::ToString(m_col_common.add.ToRGBA())));
	}
	if (m_type & simp::NodeTrans::COL_R_MASK) {
		colors.push_back(lua::assign("rmap", ee::StringHelper::ToString(m_col_map.rmap.ToRGBA())));
	}
	if (m_type & simp::NodeTrans::COL_G_MASK) {
		colors.push_back(lua::assign("gmap", ee::StringHelper::ToString(m_col_map.gmap.ToRGBA())));
	}
	if (m_type & simp::NodeTrans::COL_B_MASK) {
		colors.push_back(lua::assign("bmap", ee::StringHelper::ToString(m_col_map.bmap.ToRGBA())));
	}
	lua::connect(gen, colors);

	// shader
	std::vector<std::string> shaders;
	if (m_type & simp::NodeTrans::BLEND_MASK) {
		shaders.push_back(lua::assign("blend", ee::StringHelper::ToString((int)m_blend)));
	}
	if (m_type & simp::NodeTrans::FAST_BLEND_MASK) {
		shaders.push_back(lua::assign("fast_blend", ee::StringHelper::ToString((int)m_fast_blend)));
	}
	if (m_type & simp::NodeTrans::FILTER_MASK) {
		shaders.push_back(lua::assign("filter", ee::StringHelper::ToString((int)(m_filter->GetMode()))));
	}
	if (m_type & simp::NodeTrans::DOWNSMAPLE_MASK) {
		shaders.push_back(lua::assign("downsample", ee::StringHelper::ToString(m_downsample)));
	}
	if (m_type & simp::NodeTrans::CAMERA_MASK) {
		shaders.push_back(lua::assign("camera", ee::StringHelper::ToString((int)(m_camera.GetMode()))));
	}
	lua::connect(gen, shaders);

	// other
	if (m_need_actor) {
		lua::connect(gen, 1, lua::assign("need_actor", m_need_actor));
	}	
	if (m_integrate) {
		lua::connect(gen, 1, lua::assign("integrate", m_integrate));
	}	

	if (!m_name.empty()) {
		lua::connect(gen, 1, lua::assign("name", "\""+m_name+"\""));
	}
}

int PackTrans::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeTrans::Size();
	if (m_type & simp::NodeTrans::SCALE_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (m_type & simp::NodeTrans::SHEAR_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (m_type & simp::NodeTrans::OFFSET_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (m_type & simp::NodeTrans::POSITION_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (m_type & simp::NodeTrans::ANGLE_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::COL_MUL_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::COL_ADD_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::COL_R_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::COL_G_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::COL_B_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::BLEND_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::FAST_BLEND_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::FILTER_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::DOWNSMAPLE_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::CAMERA_MASK) {
		sz += sizeof(uint32_t);
	}
	sz += sizeof_unpack_str(m_name);
	return sz;
}

int PackTrans::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);		// type
	if (m_type & simp::NodeTrans::SCALE_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (m_type & simp::NodeTrans::SHEAR_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (m_type & simp::NodeTrans::OFFSET_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (m_type & simp::NodeTrans::POSITION_MASK) {
		sz += sizeof(uint32_t) * 2;
	}
	if (m_type & simp::NodeTrans::ANGLE_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::COL_MUL_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::COL_ADD_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::COL_R_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::COL_G_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::COL_B_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::BLEND_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::FAST_BLEND_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::FILTER_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::DOWNSMAPLE_MASK) {
		sz += sizeof(uint32_t);
	}
	if (m_type & simp::NodeTrans::CAMERA_MASK) {
		sz += sizeof(uint32_t);
	}
	sz += sizeof_pack_str(m_name);
	return sz;
}

void PackTrans::PackToBin(uint8_t** ptr) const
{
	pack(m_type, ptr);
	if (m_type & simp::NodeTrans::SCALE_MASK) {
		int x = ToInt(m_scale.x),
			y = ToInt(m_scale.y);
		pack(x, ptr);
		pack(y, ptr);
	}
	if (m_type & simp::NodeTrans::SHEAR_MASK) {
		int x = ToInt(m_shear.x),
			y = ToInt(m_shear.y);
		pack(x, ptr);
		pack(y, ptr);
	}
	if (m_type & simp::NodeTrans::OFFSET_MASK) {
		int x = ToInt(m_offset.x),
			y = ToInt(m_offset.y);
		pack(x, ptr);
		pack(y, ptr);
	}
	if (m_type & simp::NodeTrans::POSITION_MASK) {
		int x = ToInt(m_position.x),
			y = ToInt(m_position.y);
		pack(x, ptr);
		pack(y, ptr);
	}
	if (m_type & simp::NodeTrans::ANGLE_MASK) {
		int angle = ToInt(m_angle);
		pack(angle, ptr);
	}
	if (m_type & simp::NodeTrans::COL_MUL_MASK) {
		uint32_t col = m_col_common.mul.ToRGBA();
		pack(col, ptr);
	}
	if (m_type & simp::NodeTrans::COL_ADD_MASK) {
		uint32_t col = m_col_common.add.ToRGBA();
		pack(col, ptr);
	}
	if (m_type & simp::NodeTrans::COL_R_MASK) {
		uint32_t col = m_col_map.rmap.ToRGBA();
		pack(col, ptr);
	}
	if (m_type & simp::NodeTrans::COL_G_MASK) {
		uint32_t col = m_col_map.gmap.ToRGBA();
		pack(col, ptr);
	}
	if (m_type & simp::NodeTrans::COL_B_MASK) {
		uint32_t col = m_col_map.bmap.ToRGBA();
		pack(col, ptr);
	}
	if (m_type & simp::NodeTrans::BLEND_MASK) {
		uint32_t mode = m_blend;
		pack(mode, ptr);
	}
	if (m_type & simp::NodeTrans::FAST_BLEND_MASK) {
		uint32_t mode = m_fast_blend;
		pack(mode, ptr);
	}
	if (m_type & simp::NodeTrans::FILTER_MASK) {
		uint32_t mode = m_filter->GetMode();
		pack(mode, ptr);
	}
	if (m_type & simp::NodeTrans::DOWNSMAPLE_MASK) {
		uint32_t ds = m_downsample * 0xffffffff;
		pack(ds, ptr);
	}
	if (m_type & simp::NodeTrans::CAMERA_MASK) {
		uint32_t mode = m_camera.GetMode();
		pack(mode, ptr);
	}
	pack_str(m_name, ptr);
}

}
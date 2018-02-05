#include "s2loader/SpriteIO.h"
#include "s2loader/BlendModes.h"
#include "s2loader/CameraModes.h"
#include "s2loader/FastBlendModes.h"
#include "s2loader/FilterModes.h"

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/HeatHazeProg.h>
#include <shaderlab/FilterShader.h>
#include <sprite2/Sprite.h>
#include <sprite2/StatImages.h>
#include <bs/FixedPointNum.h>
#include <gum/FilepathHelper.h>
#include <gum/trans_color.h>
#include <gum/Image.h>
#include <gum/ImagePool.h>
#include <painting2/RenderCamera.h>
#include <painting2/RenderShader.h>
#include <painting2/FilterFactory.h>
#include <painting2/RFEdgeDetection.h>
#include <painting2/RFGaussianBlur.h>
#include <painting2/RFOuterGlow.h>
#include <painting2/RFHeatHaze.h>

namespace s2loader
{

SpriteIO::SpriteIO(bool m_compress, bool render_open)
#ifdef S2_FILTER_FULL
	: m_filter(nullptr, pt2::RenderFilter::Deleter)
#else
	: m_filter(pt2::FM_NULL)
#endif // S2_FILTER_FULL
	, m_compress(m_compress)
	, m_render_open(render_open)
{
	m_position		= sm::vec2(0, 0);
	m_angle			= 0;
	m_scale			= sm::vec2(1, 1);
	m_shear			= sm::vec2(0, 0);
//	m_offset.MakeInvalid();
	m_offset        = sm::vec2(0, 0);

	m_blend			= pt2::BM_NULL;
	m_fast_blend	= pt2::FBM_NULL;
	m_camera		= pt2::CM_ORTHO;
	m_downsample    = 1;

	m_need_actor    = false;
	m_integrate     = false;

	m_visible		= true;
	m_editable		= true;
}

void SpriteIO::Load(const Json::Value& val, const s2::SprPtr& spr, const CU_STR& dir)
{
	Load(val, dir);

	LoadGeometry(spr);
	LoadRender(spr);
	LoadInfo(spr);
	LoadEdit(spr);
}

void SpriteIO::Store(Json::Value& val, const s2::SprConstPtr& spr, const CU_STR& dir)
{
	StoreGeometry(spr);
	StoreRender(spr);
	StoreInfo(spr);
	StoreEdit(spr);

	Store(val, dir);
}

void SpriteIO::Load(const Json::Value& val, const CU_STR& dir)
{
	LoadGeometry(val);
	LoadRender(val, dir);
	LoadInfo(val);
	LoadEdit(val);
}

void SpriteIO::Store(Json::Value& val, const CU_STR& dir)
{
	StoreGeometry(val);
	StoreRender(val, dir);
	StoreInfo(val);
	StoreEdit(val);
}

void SpriteIO::Load(const s2s::NodeSprCommon& node_spr, const s2::SprPtr& spr)
{
	Load(node_spr);

	LoadGeometry(spr);
	LoadRender(spr);
	LoadInfo(spr);
	LoadEdit(spr);
}

void SpriteIO::Load(const s2s::NodeSprCommon& node_spr)
{
	m_name = node_spr.GetName();

	uint32_t type = node_spr.GetType();
	const uint32_t* data = node_spr.GetData();
	
	int idx = 0;

	if (type & s2s::NodeSprCommon::SCALE_MASK) {
		m_scale.x = bs::int2float(data[idx++], s2s::NodeSprCommon::HIGH_FIXED_TRANS_PRECISION);
		m_scale.y = bs::int2float(data[idx++], s2s::NodeSprCommon::HIGH_FIXED_TRANS_PRECISION);
	}
	if (type & s2s::NodeSprCommon::SHEAR_MASK) {
		m_shear.x = bs::int2float(data[idx++], s2s::NodeSprCommon::HIGH_FIXED_TRANS_PRECISION);
		m_shear.y = bs::int2float(data[idx++], s2s::NodeSprCommon::HIGH_FIXED_TRANS_PRECISION);
	}
	if (type & s2s::NodeSprCommon::OFFSET_MASK) {
		m_offset.x = bs::int2float(data[idx++], s2s::NodeSprCommon::LOW_FIXED_TRANS_PRECISION);
		m_offset.y = bs::int2float(data[idx++], s2s::NodeSprCommon::LOW_FIXED_TRANS_PRECISION);
	} 
	if (type & s2s::NodeSprCommon::POSITION_MASK) {
		m_position.x = bs::int2float(data[idx++], s2s::NodeSprCommon::LOW_FIXED_TRANS_PRECISION);
		m_position.y = bs::int2float(data[idx++], s2s::NodeSprCommon::LOW_FIXED_TRANS_PRECISION);
	} 
	if (type & s2s::NodeSprCommon::ANGLE_MASK) {
		m_angle = bs::int2float(data[idx++], s2s::NodeSprCommon::HIGH_FIXED_TRANS_PRECISION);
	} 

	if (type & s2s::NodeSprCommon::COL_MUL_MASK) {
		pt2::Color col;
		col.FromRGBA(data[idx++]);
		m_col_common.mul = col;
	} 
	if (type & s2s::NodeSprCommon::COL_ADD_MASK) {
		pt2::Color col;
		col.FromRGBA(data[idx++]);
		m_col_common.add = col;
	} 
	if (type & s2s::NodeSprCommon::COL_R_MASK) {
		pt2::Color col;
		col.FromRGBA(data[idx++]);
		m_col_map.rmap = col;
	} 
	if (type & s2s::NodeSprCommon::COL_G_MASK) {
		pt2::Color col;
		col.FromRGBA(data[idx++]);
		m_col_map.gmap = col;
	} 
	if (type & s2s::NodeSprCommon::COL_B_MASK) {
		pt2::Color col;
		col.FromRGBA(data[idx++]);
		m_col_map.bmap = col;
	}
}

/************************************************************************/
/* geometry                                                             */
/************************************************************************/

void SpriteIO::LoadGeometry(const s2::SprPtr& spr)
{
	spr->SetShear(m_shear);
	spr->SetScale(m_scale);
	spr->SetOffset(m_offset);
	spr->SetPosition(m_position);
	spr->SetAngle(m_angle);
}

void SpriteIO::StoreGeometry(const s2::SprConstPtr& spr)
{
	m_position	= spr->GetPosition();
	m_angle		= spr->GetAngle();
	m_scale		= spr->GetScale();
	m_shear		= spr->GetShear();
	m_offset	= spr->GetOffset();
}

void SpriteIO::LoadGeometry(const Json::Value& val)
{
	// scale
	sm::vec2 scale(1, 1);
	if (val.isMember("x scale") && val.isMember("y scale"))
	{
		scale.x = static_cast<float>(val["x scale"].asDouble());
		scale.y = static_cast<float>(val["y scale"].asDouble());
	}
	else if (val.isMember("scale"))
	{
		scale.x = scale.y = static_cast<float>(val["scale"].asDouble());
	}
	m_scale = scale;

	// shear
	sm::vec2 shear(0, 0);
	if (val.isMember("x shear") && val.isMember("y shear"))
	{
		shear.x = static_cast<float>(val["x shear"].asDouble());
		shear.y = static_cast<float>(val["y shear"].asDouble());
	}
	m_shear = shear;

	// offset
	sm::vec2 offset(0 ,0);
	if (val.isMember("x offset") && val.isMember("y offset"))
	{
		offset.x = static_cast<float>(val["x offset"].asDouble());
		offset.y = static_cast<float>(val["y offset"].asDouble());
	}
	m_offset = offset;

	// translate
	sm::vec2 pos(0, 0);
	if (val.isMember("position") && val["position"].isMember("x") && val["position"].isMember("y")) {
		pos.x = static_cast<float>(val["position"]["x"].asDouble());
		pos.y = static_cast<float>(val["position"]["y"].asDouble());
	}
	m_position = pos;

	// rotate
	float angle = 0;
	if (val.isMember("angle")) {
		angle = static_cast<float>(val["angle"].asDouble());
	}
	m_angle = angle;
}

void SpriteIO::StoreGeometry(Json::Value& val)
{
	if (!m_compress || m_position != sm::vec2(0, 0)) {
		val["position"]["x"] = m_position.x;
		val["position"]["y"] = m_position.y;
	}

	if (!m_compress || m_angle != 0) {
		val["angle"] = m_angle;
	}

	if (!m_compress || m_scale != sm::vec2(1, 1)) {
		val["x scale"] = m_scale.x;
		val["y scale"] = m_scale.y;
	}

	if (!m_compress || m_shear != sm::vec2(0, 0)) {
		val["x shear"] = m_shear.x;
		val["y shear"] = m_shear.y;
	}

	if (!m_compress || m_offset.IsValid()) {
		val["x offset"] = m_offset.x;
		val["y offset"] = m_offset.y;
	}
}

/************************************************************************/
/* render                                                               */
/************************************************************************/

void SpriteIO::LoadRender(const s2::SprPtr& spr)
{
	LoadColor(spr);
	LoadShader(spr);
	LoadCamera(spr);	
}

void SpriteIO::StoreRender(const s2::SprConstPtr& spr)
{
	StoreColor(spr->GetColorCommon(), spr->GetColorMap());
	StoreShader(spr->GetShader());
	StoreCamera(spr->GetCamera());	
}

void SpriteIO::LoadRender(const Json::Value& val, const CU_STR& dir)
{
	LoadColor(val);
	LoadShader(val, dir);
	LoadCamera(val);
}

void SpriteIO::StoreRender(Json::Value& val, const CU_STR& dir)
{
	StoreColor(val);
	StoreShader(val, dir);
	StoreCamera(val);	
}

/************************************************************************/
/* info                                                                 */
/************************************************************************/

void SpriteIO::LoadInfo(const s2::SprPtr& spr)
{
	spr->SetName(m_name);
	spr->SetNeedActor(m_need_actor);
	spr->SetIntegrate(m_integrate);
}

void SpriteIO::StoreInfo(const s2::SprConstPtr& spr)
{
	s2::SprNameMap::Instance()->IDToStr(spr->GetName(), m_name);
	m_need_actor = spr->IsNeedActor();
	m_integrate  = spr->IsIntegrate();
}

void SpriteIO::LoadInfo(const Json::Value& val)
{
	if (val.isMember("name")) {
		m_name = val["name"].asString().c_str();
	} else {
		m_name = "";
	}
	if (val.isMember("actor")) {
		m_need_actor = val["actor"].asBool();
	} else {
		m_need_actor = false;
	}
	if (val.isMember("integrate")) {
		m_integrate = val["integrate"].asBool();
	} else {
		m_integrate = false;
	}
}

void SpriteIO::StoreInfo(Json::Value& val)
{
	if (!m_compress || !m_name.empty()) {
		val["name"] = m_name.c_str();
	}
	if (!m_compress || m_need_actor) {
		val["actor"] = m_need_actor;
	}
	if (!m_compress || m_integrate) {
		val["integrate"] = m_integrate;
	}
}

/************************************************************************/
/* edit                                                                 */
/************************************************************************/

void SpriteIO::LoadEdit(const s2::SprPtr& spr)
{
	spr->SetVisible(m_visible);
	spr->SetEditable(m_editable);
}

void SpriteIO::StoreEdit(const s2::SprConstPtr& spr)
{
	m_visible = spr->IsVisible();
	m_editable = spr->IsEditable();
}

void SpriteIO::LoadEdit(const Json::Value& val)
{
	if (val.isMember("visible")) {
		m_visible = val["visible"].asBool();
	} else {
		m_visible = true;
	}
	if (val.isMember("editable")) {
		m_editable = val["editable"].asBool();
	} else {
		m_editable = true;
	}
}

void SpriteIO::StoreEdit(Json::Value& val)
{
	if (!m_compress || !m_visible) {
		val["visible"] = m_visible;
	}
	if (!m_compress || !m_editable) {
		val["editable"] = m_editable;
	}
}

/************************************************************************/
/* private                                                              */
/************************************************************************/

void SpriteIO::LoadColor(const s2::SprPtr& spr)
{
	spr->SetColorCommon(m_col_common);
	spr->SetColorMap(m_col_map);
}

void SpriteIO::StoreColor(const pt2::RenderColorCommon& col_common, const pt2::RenderColorMap& col_map)
{
	m_col_common = col_common;
	m_col_map    = col_map;
}

void SpriteIO::LoadColor(const Json::Value& val)
{
	m_col_common.mul = pt2::Color(255, 255, 255, 255);
	if (val.isMember("multi color")) {
		CU_STR str = val["multi color"].asString().c_str();
		if (!str.empty()) {
			m_col_common.mul = gum::str2color(str, s2s::BGRA);
		}
	}

	m_col_common.add = pt2::Color(0, 0, 0, 0);
	if (val.isMember("add color")) {
		CU_STR str = val["add color"].asString().c_str();
		if (!str.empty()) {
			m_col_common.add = gum::str2color(str, s2s::ABGR);
		}
	}

	m_col_map.rmap = (pt2::Color(255, 0, 0, 0));
	if (val.isMember("r trans")) {
		CU_STR str = val["r trans"].asString().c_str();
		if (!str.empty()) {
			pt2::Color col(gum::str2color(str, s2s::RGBA));
			col.a = 0;
			m_col_map.rmap = col;
		}
	}

	m_col_map.gmap = pt2::Color(0, 255, 0, 0);
	if (val.isMember("g trans")) {
		CU_STR str = val["g trans"].asString().c_str();
		if (!str.empty()) {
			pt2::Color col(gum::str2color(str, s2s::RGBA));
			col.a = 0;
			m_col_map.gmap = col;
		}
	}

	m_col_map.bmap = pt2::Color(0, 0, 255, 0);
	if (val.isMember("b trans")) {
		CU_STR str = val["b trans"].asString().c_str();
		if (!str.empty()) {
			pt2::Color col(gum::str2color(str, s2s::RGBA));
			col.a = 0;
			m_col_map.bmap = col;
		}
	}
}

void SpriteIO::StoreColor(Json::Value& val)
{
	if (!m_compress || m_col_common.mul != pt2::Color(255, 255, 255, 255)) {
		val["multi color"]	= gum::color2str(m_col_common.mul, s2s::BGRA).c_str();;
	}
	if (!m_compress || m_col_common.add != pt2::Color(0, 0, 0, 0)) {
		val["add color"]	= gum::color2str(m_col_common.add, s2s::ABGR).c_str();;
	}

	if (!m_compress || m_col_map.rmap.r != 255 || m_col_map.rmap.g != 0 || m_col_map.rmap.b != 0) {
		val["r trans"]		= gum::color2str(m_col_map.rmap, s2s::RGBA).c_str();;
	}
	if (!m_compress || m_col_map.gmap.r != 0 || m_col_map.gmap.g != 255 || m_col_map.gmap.b != 0) {
		val["g trans"]		= gum::color2str(m_col_map.gmap, s2s::RGBA).c_str();;
	}
	if (!m_compress || m_col_map.bmap.r != 0 || m_col_map.bmap.g != 0 || m_col_map.bmap.b != 255) {
		val["b trans"]		= gum::color2str(m_col_map.bmap, s2s::RGBA).c_str();;
	}
}

void SpriteIO::LoadShader(const s2::SprPtr& spr)
{
	pt2::RenderShader rs = spr->GetShader();
	rs.SetBlend(m_blend);
	rs.SetFastBlend(m_fast_blend);
#ifdef S2_FILTER_FULL
	rs.SetFilter(m_filter.get());
#else
	rs.SetFilter(m_filter);
#endif // S2_FILTER_FULL
	rs.SetDownsample(m_downsample);
	spr->SetShader(rs);
}

void SpriteIO::StoreShader(const pt2::RenderShader& shader)
{
	m_blend      = shader.GetBlend();
	m_fast_blend = shader.GetFastBlend();
#ifdef S2_FILTER_FULL
	if (shader.GetFilter()) {
		m_filter.reset(shader.GetFilter()->Clone());
	} else {
		m_filter.reset();
	}
#else
	m_filter = shader.GetFilter();
#endif // S2_FILTER_FULL
	m_downsample = shader.GetDownsample();
}

void SpriteIO::LoadShader(const Json::Value& val, const CU_STR& dir)
{
	m_blend = pt2::BM_NULL;
	m_fast_blend = pt2::FBM_NULL;
#ifdef S2_FILTER_FULL
	if (m_filter) {
		m_filter.reset();
	}
#else
	m_filter = pt2::FM_NULL;
#endif // S2_FILTER_FULL
	m_downsample = 1;

	if (!m_render_open) {
		return;
	}

	if (val.isMember("blend")) {
		CU_STR disc = val["blend"].asString().c_str();
		m_blend = BlendModes::Instance()->Name2Mode(disc);
	}

	if (val.isMember("fast_blend")) {
		CU_STR disc = val["fast_blend"].asString().c_str();
		m_fast_blend = FastBlendModes::Instance()->Name2Mode(disc);
	}

#ifdef S2_FILTER_FULL
	if (val.isMember("filter")) 
	{
		if (val["filter"].isString()) 
		{
			CU_STR disc = val["filter"].asString().c_str();
			pt2::FilterMode mode = FilterModes::Instance()->Name2Mode(disc);
			m_filter = pt2::FilterFactory::Instance()->Create(mode);
		} 
		else 
		{
			const Json::Value& fval = val["filter"];
			CU_STR disc = fval["mode"].asString().c_str();
			pt2::FilterMode mode = FilterModes::Instance()->Name2Mode(disc);
			m_filter = pt2::FilterFactory::Instance()->Create(mode);
			switch (mode)
			{
			case pt2::FM_EDGE_DETECTION:
				{
					float blend = (float)(fval["blend"].asDouble());
					auto filter = static_cast<pt2::RFEdgeDetection*>(m_filter.get());
					filter->SetBlend(blend);
				}
				break;
			case pt2::FM_GAUSSIAN_BLUR:
				{
					int iterations = fval["iterations"].asInt();
					auto filter = static_cast<pt2::RFGaussianBlur*>(m_filter.get());
					filter->SetIterations(iterations);
				}
				break;
			case pt2::FM_OUTER_GLOW:
				{
					int iterations = fval["iterations"].asInt();
					auto filter = static_cast<pt2::RFOuterGlow*>(m_filter.get());
					filter->SetIterations(iterations);
				}
				break;
			case pt2::FM_HEAT_HAZE:
				{
					auto filter = static_cast<pt2::RFHeatHaze*>(m_filter.get());
					float distortion = 0.02f, rise = 0.2f;
					if (fval.isMember("distortion")) {
						distortion = static_cast<float>(fval["distortion"].asDouble());
					}
					if (fval.isMember("rise")) {
						rise = static_cast<float>(fval["rise"].asDouble());
					}
					filter->SetFactor(distortion, rise);
					if (fval.isMember("filepath")) 
					{
						CU_STR filepath = fval["filepath"].asString().c_str();
						filepath = gum::FilepathHelper::Absolute(dir, filepath);
						filter->SetFilepath(filepath);
						
						sl::HeatHazeProg* prog = nullptr;
						sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
						sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
						if (shader) {
							prog = static_cast<sl::HeatHazeProg*>(shader->GetProgram(sl::FM_HEAT_HAZE));
						}
						if (prog) {
							auto img = gum::ImagePool::Instance()->Create(s2::StatImages::UNKNOWN_IMG_ID, bimp::FilePath(filepath));
							prog->SetDistortionMapTex(img->GetTexID());
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}
	else
	{
		m_filter = pt2::FilterFactory::Instance()->Create(pt2::FM_NULL);
	}
#endif // S2_FILTER_FULL

	if (val.isMember("downsample")) {
		m_downsample = static_cast<float>(val["downsample"].asDouble());
	}
}

void SpriteIO::StoreShader(Json::Value& val, const CU_STR& dir)
{
	if (!m_render_open) {
		return;
	}

	if (m_blend != pt2::BM_NULL) {
		val["blend"] = BlendModes::Instance()->ModeToName(m_blend).c_str();;
	}

	if (m_fast_blend != pt2::FBM_NULL) {
		val["fast_blend"] = FastBlendModes::Instance()->Mode2Name(m_fast_blend).c_str();;
	}

	pt2::FilterMode mode = pt2::FM_NULL;
#ifdef S2_FILTER_FULL
	if (m_filter) {
		mode = m_filter->GetMode();
	}
	if (mode != pt2::FM_NULL)
	{
		Json::Value fval;
		fval["mode"] = FilterModes::Instance()->Mode2Name(mode).c_str();;
		switch (mode)
		{
		case pt2::FM_EDGE_DETECTION:
			{
				auto filter = static_cast<pt2::RFEdgeDetection*>(m_filter.get());
				fval["blend"] = filter->GetBlend();
			}
			break;
		case pt2::FM_GAUSSIAN_BLUR:
			{
				auto filter = static_cast<pt2::RFGaussianBlur*>(m_filter.get());
				fval["iterations"] = filter->GetIterations();
			}
			break;
		case pt2::FM_OUTER_GLOW:
			{
				auto filter = static_cast<pt2::RFOuterGlow*>(m_filter.get());
				fval["iterations"] = filter->GetIterations();
			}
			break;
		case pt2::FM_HEAT_HAZE:
			{
				auto filter = static_cast<pt2::RFHeatHaze*>(m_filter.get());
				fval["filepath"] = gum::FilepathHelper::Relative(dir, filter->GetFilepath()).c_str();;
				float distortion, rise;
				filter->GetFactor(distortion, rise);
				fval["distortion"] = distortion;
				fval["rise"] = rise;
			}
			break;
		default:
			break;
		}
		val["filter"] = fval;
	}
#endif // S2_FILTER_FULL

	if (m_downsample != 1) {
		val["downsample"] = m_downsample;
	}
}

void SpriteIO::LoadCamera(const s2::SprPtr& spr)
{
	pt2::RenderCamera rc = spr->GetCamera();
	rc.SetMode(m_camera);
	spr->SetCamera(rc);
}

void SpriteIO::StoreCamera(const pt2::RenderCamera& rc)
{
	m_camera = rc.GetMode();
}

void SpriteIO::LoadCamera(const Json::Value& val)
{
	if (val.isMember("camera")) {
		CU_STR disc = val["camera"].asString().c_str();
		m_camera = CameraModes::Instance()->Name2Mode(disc);
	} else {
		m_camera = pt2::CM_ORTHO;
	}
}

void SpriteIO::StoreCamera(Json::Value& val)
{
	if (m_camera != pt2::CM_ORTHO) {
		val["camera"] = CameraModes::Instance()->Mode2Name(m_camera).c_str();;
	}
}

}
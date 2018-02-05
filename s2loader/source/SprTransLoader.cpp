#include "s2loader/SprTransLoader.h"

#include <s2s/ColorParser.h>
#include <s2s/NodeSprCommon.h>
#include <sprite2/Sprite.h>
#include <sprite2/Symbol.h>
#include <simp/NodeTrans.h>
#include <painting2/RenderCamera.h>
#include <painting2/RenderShader.h>

namespace s2loader
{

void SprTransLoader::Load(const s2::SprPtr& spr, const simp::NodeTrans* trans)
{
	if (!spr) {
		return;
	}

	int idx = 0;
	if (trans->type & simp::NodeTrans::SCALE_MASK) {
		float x = ToFloat(trans->data[idx++]),
			  y = ToFloat(trans->data[idx++]);
		spr->SetScale(sm::vec2(x, y));
	}
	if (trans->type & simp::NodeTrans::SHEAR_MASK) {
		float x = ToFloat(trans->data[idx++]),
			  y = ToFloat(trans->data[idx++]);
		spr->SetShear(sm::vec2(x, y));
	}
	if (trans->type & simp::NodeTrans::OFFSET_MASK) {
		float x = ToFloat(trans->data[idx++]),
			  y = ToFloat(trans->data[idx++]);
		spr->SetOffset(sm::vec2(x, y));
	}
	if (trans->type & simp::NodeTrans::POSITION_MASK) {
		float x = ToFloat(trans->data[idx++]),
			  y = ToFloat(trans->data[idx++]);
		spr->SetPosition(sm::vec2(x, y));
	}
	if (trans->type & simp::NodeTrans::ANGLE_MASK) {
		float angle = ToFloat(trans->data[idx++]);
		spr->SetAngle(angle);
	}

	if (trans->type & simp::NodeTrans::COL_MUL_MASK) 
	{
		pt2::Color mul;
		mul.FromRGBA(trans->data[idx++]);
		auto& col_common = spr->GetColorCommon();
		spr->SetColorCommon(pt2::RenderColorCommon(mul, col_common.add));
	}
	if (trans->type & simp::NodeTrans::COL_ADD_MASK) 
	{
		pt2::Color add;
		add.FromRGBA(trans->data[idx++]);
		auto& col_common = spr->GetColorCommon();
		spr->SetColorCommon(pt2::RenderColorCommon(col_common.mul, add));
	}
	if (trans->type & simp::NodeTrans::COL_R_MASK) 
	{
		pt2::Color rmap;
		rmap.FromRGBA(trans->data[idx++]);
		auto& col_map = spr->GetColorMap();
		spr->SetColorMap(pt2::RenderColorMap(rmap, col_map.gmap, col_map.bmap));
	}
	if (trans->type & simp::NodeTrans::COL_G_MASK) 
	{
		pt2::Color gmap;
		gmap.FromRGBA(trans->data[idx++]);
		auto& col_map = spr->GetColorMap();
		spr->SetColorMap(pt2::RenderColorMap(col_map.rmap, gmap, col_map.bmap));
	}
	if (trans->type & simp::NodeTrans::COL_B_MASK) 
	{
		pt2::Color bmap;
		bmap.FromRGBA(trans->data[idx++]);
		auto& col_map = spr->GetColorMap();
		spr->SetColorMap(pt2::RenderColorMap(col_map.rmap, col_map.gmap, bmap));
	}

	pt2::RenderShader rs;
	if (trans->type & simp::NodeTrans::BLEND_MASK) {
		rs.SetBlend(pt2::BlendMode(trans->data[idx++]));
	}
	if (trans->type & simp::NodeTrans::FAST_BLEND_MASK) {
		rs.SetFastBlend(pt2::FastBlendMode(trans->data[idx++]));
	}
	if (trans->type & simp::NodeTrans::FILTER_MASK) {
		pt2::FilterMode mode = pt2::FilterMode(trans->data[idx++]);
		rs.SetFilter(mode);
	}
	if (trans->type & simp::NodeTrans::DOWNSMAPLE_MASK) {
		float downsample = static_cast<float>(trans->data[idx++]) / 0xffffffff;
		rs.SetDownsample(downsample);
	}
	spr->SetShader(rs);

	if (trans->type & simp::NodeTrans::CAMERA_MASK) {
		pt2::RenderCamera rc;
		rc.SetMode(pt2::CameraMode(trans->data[idx++]));
		spr->SetCamera(rc);
	}

	spr->SetNeedActor((trans->type & simp::NodeTrans::ACTOR_MASK) != 0);
	spr->SetIntegrate((trans->type & simp::NodeTrans::INTEGRATE_MASK) != 0);	

	if (trans->name) {
		spr->SetName(trans->name);
	}
}

void SprTransLoader::Load(const s2::SprPtr& spr, const s2s::NodeSprCommon& common)
{
	if (!spr) {
		return;
	}

	uint32_t type = common.GetType();
	const uint32_t* data = common.GetData();
	int idx = 0;
	if (type & s2s::NodeSprCommon::SCALE_MASK) {
		float x = ToFloat(data[idx++], s2s::NodeSprCommon::HIGH_FIXED_TRANS_PRECISION),
			  y = ToFloat(data[idx++], s2s::NodeSprCommon::HIGH_FIXED_TRANS_PRECISION);
		spr->SetScale(sm::vec2(x, y));
	} 
	if (type & s2s::NodeSprCommon::SHEAR_MASK) {
		float x = ToFloat(data[idx++], s2s::NodeSprCommon::HIGH_FIXED_TRANS_PRECISION),
			  y = ToFloat(data[idx++], s2s::NodeSprCommon::HIGH_FIXED_TRANS_PRECISION);
		spr->SetShear(sm::vec2(x, y));
	} 
	if (type & s2s::NodeSprCommon::OFFSET_MASK) {
		float x = ToFloat(data[idx++], s2s::NodeSprCommon::LOW_FIXED_TRANS_PRECISION),
			  y = ToFloat(data[idx++], s2s::NodeSprCommon::LOW_FIXED_TRANS_PRECISION);
		spr->SetOffset(sm::vec2(x, y));
	} 
	if (type & s2s::NodeSprCommon::POSITION_MASK) {
		float x = ToFloat(data[idx++], s2s::NodeSprCommon::LOW_FIXED_TRANS_PRECISION),
			  y = ToFloat(data[idx++], s2s::NodeSprCommon::LOW_FIXED_TRANS_PRECISION);
		spr->SetPosition(sm::vec2(x, y));
	}
	if (type & s2s::NodeSprCommon::ANGLE_MASK) {
		float angle = ToFloat(data[idx++], s2s::NodeSprCommon::HIGH_FIXED_TRANS_PRECISION);
		spr->SetAngle(angle);
	} 


	if (type & simp::NodeTrans::COL_MUL_MASK)
	{
		pt2::Color mul;
		mul.FromRGBA(data[idx++]);
		auto& col_common = spr->GetColorCommon();
		spr->SetColorCommon(pt2::RenderColorCommon(mul, col_common.add));
	}
	if (type & simp::NodeTrans::COL_ADD_MASK)
	{
		pt2::Color add;
		add.FromRGBA(data[idx++]);
		auto& col_common = spr->GetColorCommon();
		spr->SetColorCommon(pt2::RenderColorCommon(col_common.mul, add));
	}
	if (type & simp::NodeTrans::COL_R_MASK)
	{
		pt2::Color rmap;
		rmap.FromRGBA(data[idx++]);
		auto& col_map = spr->GetColorMap();
		spr->SetColorMap(pt2::RenderColorMap(rmap, col_map.gmap, col_map.bmap));
	}
	if (type & simp::NodeTrans::COL_G_MASK)
	{
		pt2::Color gmap;
		gmap.FromRGBA(data[idx++]);
		auto& col_map = spr->GetColorMap();
		spr->SetColorMap(pt2::RenderColorMap(col_map.rmap, gmap, col_map.bmap));
	}
	if (type & simp::NodeTrans::COL_B_MASK)
	{
		pt2::Color bmap;
		bmap.FromRGBA(data[idx++]);
		auto& col_map = spr->GetColorMap();
		spr->SetColorMap(pt2::RenderColorMap(col_map.rmap, col_map.gmap, bmap));
	}

	// todo
	//pt2::RenderShader rs;
	//if (type & simp::NodeTrans::BLEND_MASK) {
	//	rs.SetBlend(pt2::BlendMode(data[idx++]));
	//}
	//if (type & simp::NodeTrans::FAST_BLEND_MASK) {
	//	rs.SetFastBlend(pt2::FastBlendMode(data[idx++]));
	//}
	//if (type & simp::NodeTrans::FILTER_MASK) {
	//	pt2::FilterMode mode = pt2::FilterMode(data[idx++]);
	//	rs.SetFilter(mode);
	//}
	//if (type & simp::NodeTrans::DOWNSMAPLE_MASK) {
	//	float downsample = static_cast<float>(data[idx++]) / 0xffffffff;
	//	rs.SetDownsample(downsample);
	//}
	//spr->SetShader(rs);

	//if (type & simp::NodeTrans::CAMERA_MASK) {
	//	pt2::RenderCamera rc;
	//	rc.SetMode(pt2::CameraMode(data[idx++]));
	//	spr->SetCamera(rc);
	//}

	spr->SetNeedActor((type & s2s::NodeSprCommon::ACTOR_MASK) != 0);
	spr->SetIntegrate((type & s2s::NodeSprCommon::INTEGRATE_MASK) != 0);

	if (common.GetName()) {
		spr->SetName(common.GetName());
	}
}

float SprTransLoader::ToFloat(int i, int precision)
{
	return static_cast<float>(i) / precision;
}

}
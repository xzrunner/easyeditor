#include "SprTransLoader.h"

#include <sprite2/S2_Sprite.h>
#include <simp/NodeTrans.h>

namespace gum
{

void SprTransLoader::Load(s2::Sprite* spr, const simp::NodeTrans* trans)
{
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
	if (trans->type & simp::NodeTrans::COL_MUL_MASK) {
		spr->Color().mul.FromRGBA(trans->data[idx++]);
	}
	if (trans->type & simp::NodeTrans::COL_ADD_MASK) {
		spr->Color().add.FromRGBA(trans->data[idx++]);
	}
	if (trans->type & simp::NodeTrans::COL_R_MASK) {
		spr->Color().rmap.FromRGBA(trans->data[idx++]);
	}
	if (trans->type & simp::NodeTrans::COL_G_MASK) {
		spr->Color().gmap.FromRGBA(trans->data[idx++]);
	}
	if (trans->type & simp::NodeTrans::COL_B_MASK) {
		spr->Color().bmap.FromRGBA(trans->data[idx++]);
	}
	if (trans->type & simp::NodeTrans::BLEND_MASK) {
		spr->Shader().blend = s2::BlendMode(trans->data[idx++]);
	}
	if (trans->type & simp::NodeTrans::FAST_BLEND_MASK) {
		spr->Shader().fast_blend = s2::FastBlendMode(trans->data[idx++]);
	}
	if (trans->type & simp::NodeTrans::FILTER_MASK) {
		s2::FilterMode mode = s2::FilterMode(trans->data[idx++]);
		spr->Shader().SetFilter(mode);
	}
	if (trans->type & simp::NodeTrans::CAMERA_MASK) {
		spr->Camera().mode = s2::CameraMode(trans->data[idx++]);
	}

	if (trans->name) {
		spr->SetName(trans->name);
	}
}

float SprTransLoader::ToFloat(int i)
{
	return i / 1024.0f;
}

}
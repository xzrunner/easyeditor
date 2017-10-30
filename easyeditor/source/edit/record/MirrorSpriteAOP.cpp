#include "MirrorSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"
#include "Sprite.h"

namespace ee
{

MirrorSpriteAOP::MirrorSpriteAOP(const SprPtr& spr,
								 bool old_mirror_x, bool old_mirror_y,
								 bool new_mirror_x, bool new_mirror_y)
	: m_spr(spr)
	, m_old_mirror(old_mirror_x, old_mirror_y)
	, m_new_mirror(new_mirror_x, new_mirror_y)
{
}

void MirrorSpriteAOP::Undo()
{
	SetMirror(m_spr, m_old_mirror);
}

void MirrorSpriteAOP::Redo()
{
	SetMirror(m_spr, m_new_mirror);
}

void MirrorSpriteAOP::Copy(const std::vector<SprPtr>& sprs)
{
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		SetMirror(sprs[i], m_new_mirror);
	}	
}

Json::Value MirrorSpriteAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value val;
	val["type"] = AT_SHEAR;
	val["new_mirror_x"] = m_new_mirror.x;
	val["new_mirror_y"] = m_new_mirror.y;
	val["old_mirror_x"] = m_old_mirror.x;
	val["old_mirror_y"] = m_old_mirror.y;
	return val;
}

void MirrorSpriteAOP::SetMirror(const SprPtr& spr, const sm::bvec2& mirror)
{
	sm::vec2 scale = spr->GetScale();
	if (mirror.x) {
		scale.x = -fabs(scale.x);
	} else {
		scale.x = fabs(scale.x);
	}
	if (mirror.y) {
		scale.y = -fabs(scale.y);
	} else {
		scale.y = fabs(scale.y);
	}
	spr->SetScale(scale);
}

}
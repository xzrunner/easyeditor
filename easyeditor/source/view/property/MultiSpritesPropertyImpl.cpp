#include "MultiSpritesPropertyImpl.h"
#include "Sprite.h"
#include "Symbol.h"
#include "color_config.h"
#include "panel_msg.h"
#include "SetSpritePosAOP.h"
#include "TranslateSpriteAOP.h"

#include <SM_Calc.h>

#include <wx/string.h>

#include <algorithm>

namespace ee
{

const wxChar* MultiSpritesPropertyImpl::BOOL_3TYPE_LABELS[] 
	= { wxT("[?]"), wxT("True"), wxT("False"), NULL };		

const wxChar* MultiSpritesPropertyImpl::ALIGN_LABELS[] 
	= { wxT("无"), wxT("左"), wxT("右"), wxT("上"), wxT("下"), wxT("中心左"), wxT("中心右"), wxT("中心上"), wxT("中心下"), NULL };

const wxChar* MultiSpritesPropertyImpl::CENTER_LABELS[] 
	= { wxT("无"), wxT("水平"), wxT("竖直"), wxT("相对"), NULL };

MultiSpritesPropertyImpl::MultiSpritesPropertyImpl(const std::vector<SprPtr>& sprs)
	: m_sprs(sprs)
{
}

std::string MultiSpritesPropertyImpl::GetTag() const
{
	if (m_sprs.empty()) {
		return "";
	}

	std::string tag = m_sprs[0]->GetTag();
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (tag != m_sprs[i]->GetTag()) {
			return "[different]";
		}
	}
	return tag;
}

sm::vec2 MultiSpritesPropertyImpl::GetPosition() const
{
	if (m_sprs.empty()) {
		return sm::vec2();
	}

	sm::vec2 p = m_sprs[0]->GetPosition();
	for (size_t i = 1, n = m_sprs.size(); i < n; ++i)
	{
		if (p != m_sprs[i]->GetPosition())
		{
			p.Set(0, 0);
			break;
		}
	}
	return p;
}

pt2::Color MultiSpritesPropertyImpl::GetMultiColor() const
{
	if (m_sprs.empty()) {
		return WHITE;
	}

	auto& rc = m_sprs[0]->GetColorCommon();
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (rc.mul != m_sprs[i]->GetColorCommon().mul) {
			return WHITE;
		}
	}
	return rc.mul;
}

pt2::Color MultiSpritesPropertyImpl::GetAddColor() const
{
	if (m_sprs.empty()) {
		return BLACK;
	}
	
	auto& rc = m_sprs[0]->GetColorCommon();
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (rc.add != m_sprs[i]->GetColorCommon().add) {
			return BLACK;
		}
	}
	return rc.add;
}

pt2::Color MultiSpritesPropertyImpl::GetTransColorR() const
{
	if (m_sprs.empty()) {
		return RED;
	}

	auto& rc = m_sprs[0]->GetColorMap();
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (rc.rmap != m_sprs[i]->GetColorMap().rmap) {
			return RED;
		}
	}
	return rc.rmap;
}

pt2::Color MultiSpritesPropertyImpl::GetTransColorG() const
{
	if (m_sprs.empty()) {
		return GREEN;
	}

	auto& rc = m_sprs[0]->GetColorMap();
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (rc.gmap != m_sprs[i]->GetColorMap().gmap) {
			return GREEN;
		}
	}
	return rc.gmap;
}

pt2::Color MultiSpritesPropertyImpl::GetTransColorB() const
{
	if (m_sprs.empty()) {
		return BLUE;
	}

	auto& rc = m_sprs[0]->GetColorMap();
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (rc.bmap != m_sprs[i]->GetColorMap().bmap) {
			return BLUE;
		}
	}
	return rc.bmap;
}

float MultiSpritesPropertyImpl::GetAngle() const
{
	if (m_sprs.empty()) {
		return 0;
	}

	float angle = m_sprs[0]->GetAngle();
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (angle != m_sprs[i]->GetAngle()) {
			return 0;
		}
	}
	return angle;
}

sm::vec2 MultiSpritesPropertyImpl::GetScale() const
{
	if (m_sprs.empty()) {
		return sm::vec2(1, 1);
	}

	sm::vec2 scale = m_sprs[0]->GetScale();
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (scale != m_sprs[i]->GetScale()) {
			return sm::vec2(1, 1);
		}
	}
	return scale;
}

sm::bvec2 MultiSpritesPropertyImpl::GetMirror() const
{
	sm::bvec2 mirror(false, false);

	if (m_sprs.empty()) {
		return mirror;
	}

	mirror.x = m_sprs[0]->GetScale().x < 0;
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (mirror.x != m_sprs[i]->GetScale().x < 0) {
			mirror.x = false;
			break;
		}
	}

	mirror.y = m_sprs[0]->GetScale().y < 0;
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (mirror.y != m_sprs[i]->GetScale().y < 0) {
			mirror.y = false;
			break;
		}
	}

	return mirror;
}

bool MultiSpritesPropertyImpl::GetVisible() const
{
	if (m_sprs.empty()) {
		return true;
	}

	bool ret = m_sprs[0]->IsVisible();
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (ret != m_sprs[i]->IsVisible()) {
			return true;
		}
	}
	return ret;
}

bool MultiSpritesPropertyImpl::GetEditable() const
{
	if (m_sprs.empty()) {
		return true;
	}

	bool ret = m_sprs[0]->IsEditable();
	for (int i = 1, n = m_sprs.size(); i < n; ++i) {
		if (ret != m_sprs[i]->IsEditable()) {
			return true;
		}
	}
	return ret;
}

void MultiSpritesPropertyImpl::SetTag(const std::string& tag)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetTag(tag);
	}
}

void MultiSpritesPropertyImpl::SetPos(float x, float y)
{
	sm::vec2 pos(x, y);
	EditAddRecordSJ::Instance()->Add(std::make_shared<SetSpritePosAOP>(m_sprs, pos));
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		const SprPtr& spr = m_sprs[i];
		spr->SetPosition(pos);
	}
}

void MultiSpritesPropertyImpl::SetColorMul(const pt2::Color& col)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprs.size(); i < n; ++i) 
	{
		auto rc = m_sprs[i]->GetColorCommon();
		float old_a = rc.mul.a;
		rc.mul = col;
		rc.mul.a = old_a;
		m_sprs[i]->SetColorCommon(rc);
	}
}

void MultiSpritesPropertyImpl::SetColorAdd(const pt2::Color& col)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprs.size(); i < n; ++i) 
	{
		auto rc = m_sprs[i]->GetColorCommon();
		float old_a = rc.add.a;
		rc.add = col;
		rc.add.a = old_a;
		m_sprs[i]->SetColorCommon(rc);
	}
}

void MultiSpritesPropertyImpl::SetColorAlpha(int alpha)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprs.size(); i < n; ++i) 
	{
		auto rc = m_sprs[i]->GetColorCommon();
		rc.mul.a = alpha;
		m_sprs[i]->SetColorCommon(rc);
	}
}

void MultiSpritesPropertyImpl::SetAngle(bool overall, float angle)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall) 
	{
		sm::vec2 center = GetOverallCenter();
		for (int i = 0, n = m_sprs.size(); i < n; ++i) {
			const SprPtr& spr = m_sprs[i];
			sm::vec2 pos = sm::rotate_vector(spr->GetPosition() - center, angle);
			spr->SetPosition(pos);
			spr->SetAngle(angle);
		}
	} 
	else 
	{
		for (int i = 0, n = m_sprs.size(); i < n; ++i) {
			const SprPtr& spr = m_sprs[i];
			spr->SetAngle(angle);
		}
	}
}

void MultiSpritesPropertyImpl::SetScale(bool overall, float sx, float sy)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall)
	{
		sm::vec2 center = GetOverallCenter();
		for (int i = 0, n = m_sprs.size(); i < n; ++i) {
			const SprPtr& spr = m_sprs[i];
			sm::vec2 pos = spr->GetPosition() - center;
			pos.x *= sx;
			pos.y *= sy;
			spr->SetPosition(pos + center);
		}
	}

	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetScale(sm::vec2(sx, sy));
	}
}

void MultiSpritesPropertyImpl::SetMirrorX(bool overall, bool mirror)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall)
	{
		sm::vec2 center = GetOverallCenter();
		for (int i = 0, n = m_sprs.size(); i < n; ++i) {
			const SprPtr& spr = m_sprs[i];
			sm::vec2 pos = spr->GetPosition() - center;
			pos.x = -pos.x;
			spr->SetPosition(pos + center);
			spr->SetAngle(-spr->GetAngle());
		}
	}

	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		const SprPtr& spr = m_sprs[i];
		sm::vec2 scale = spr->GetScale();
		if (mirror) {
			scale.x = -fabs(scale.x);
		} else {
			scale.x = fabs(scale.x);
		}
		spr->SetScale(scale);
	}
}

void MultiSpritesPropertyImpl::SetMirrorY(bool overall, bool mirror)
{
	SetWndDirtySJ::Instance()->SetDirty();

	if (overall)
	{
		sm::vec2 center = GetOverallCenter();
		for (int i = 0, n = m_sprs.size(); i < n; ++i) {
			const SprPtr& spr = m_sprs[i];
			sm::vec2 pos = spr->GetPosition() - center;
			pos.y = -pos.y;
			spr->SetPosition(pos + center);
			spr->SetAngle(-spr->GetAngle());
		}
	}

	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		const SprPtr& spr = m_sprs[i];
		sm::vec2 scale = spr->GetScale();
		if (mirror) {
			scale.y = -fabs(scale.y);
		} else {
			scale.y = fabs(scale.y);
		}
		spr->SetScale(scale);
	}
}

void MultiSpritesPropertyImpl::SetVisible(bool visible)
{
	SetWndDirtySJ::Instance()->SetDirty();
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetVisible(visible);
	}
}

void MultiSpritesPropertyImpl::SetEditable(bool editable)
{
	SetWndDirtySJ::Instance()->SetDirty();
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->SetEditable(editable);
	}
}

void MultiSpritesPropertyImpl::OnAlign(int align)
{
	SetWndDirtySJ::Instance()->SetDirty();

	AlignType type = (AlignType)align;

	if (type == e_align_left)
	{
		float left = FLT_MAX;
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			float l = spr->GetBounding().GetSize().xmin;
			if (l < left)
				left = l;
		}
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			float x = spr->GetPosition().x + left - spr->GetBounding().GetSize().xmin;
			m_sprs[i]->SetPosition(sm::vec2(x, spr->GetPosition().y));
		}
	}
	else if (type == e_align_right)
	{
		float right = -FLT_MAX;
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			float r = spr->GetBounding().GetSize().xmax;
			if (r > right)
				right = r;
		}
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			float x = spr->GetPosition().x + right - spr->GetBounding().GetSize().xmax;
			m_sprs[i]->SetPosition(sm::vec2(x, spr->GetPosition().y));
		}
	}
	else if (type == e_align_up)
	{
		float up = -FLT_MAX;
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			float u = spr->GetBounding().GetSize().ymax;
			if (u > up)
				up = u;
		}
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			float y = spr->GetPosition().y + up - spr->GetBounding().GetSize().ymax;
			m_sprs[i]->SetPosition(sm::vec2(spr->GetPosition().x, y));
		}
	}
	else if (type == e_align_down)
	{
		float down = FLT_MAX;
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			float d = spr->GetBounding().GetSize().ymin;
			if (d < down)
				down = d;
		}
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			float y = spr->GetPosition().y + down - spr->GetBounding().GetSize().ymin;
			m_sprs[i]->SetPosition(sm::vec2(spr->GetPosition().x, y));
		}
	}
	else if (type == e_align_center_left)
	{
		float left = FLT_MAX;
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			float x = m_sprs[i]->GetPosition().x;
			if (x < left)
				left = x;
		}
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			m_sprs[i]->SetPosition(sm::vec2(left, spr->GetPosition().y));
		}
	}
	else if (type == e_align_center_right)
	{
		float right = -FLT_MAX;
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			float x = m_sprs[i]->GetPosition().x;
			if (x > right)
				right = x;
		}
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			m_sprs[i]->SetPosition(sm::vec2(right, spr->GetPosition().y));
		}
	}
	else if (type == e_align_center_up)
	{
		float up = -FLT_MAX;
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			float y = m_sprs[i]->GetPosition().y;
			if (y > up)
				up = y;
		}
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			m_sprs[i]->SetPosition(sm::vec2(spr->GetPosition().x, up));
		}
	}
	else if (type == e_align_center_down)
	{
		float down = FLT_MAX;
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			float y = m_sprs[i]->GetPosition().y;
			if (y < down)
				down = y;
		}
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			m_sprs[i]->SetPosition(sm::vec2(spr->GetPosition().x, down));
		}
	}
}

void MultiSpritesPropertyImpl::OnCenter(int center)
{
	SetWndDirtySJ::Instance()->SetDirty();

	CenterType type = (CenterType)center;

	if (type == e_center_horizontal)
	{
		float left = FLT_MAX, right = -FLT_MAX;
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			float x = spr->GetPosition().x;
			if (x < left) left = x;
			if (x > right) right = x;
		}

		std::vector<SprPtr> sorted(m_sprs);
		std::sort(sorted.begin(), sorted.end(), SpriteCmp(SpriteCmp::e_x));
		const float space = (right - left) / (sorted.size() - 1);
		for (size_t i = 0, n = sorted.size(); i < n; ++i)
		{
			const SprPtr& spr = sorted[i];
			spr->SetPosition(sm::vec2(left + space * i, spr->GetPosition().y));
		}
	}
	else if (type == e_center_vertical)
	{
		float down = FLT_MAX, up = -FLT_MAX;
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			float y = spr->GetPosition().y;
			if (y < down) down = y;
			if (y > up) up = y;
		}

		std::vector<SprPtr> sorted(m_sprs);
		std::sort(sorted.begin(), sorted.end(), SpriteCmp(SpriteCmp::e_y));
		const float space = (up - down) / (sorted.size() - 1);
		for (size_t i = 0, n = sorted.size(); i < n; ++i)
		{
			const SprPtr& spr = sorted[i];
			spr->SetPosition(sm::vec2(spr->GetPosition().x, down + space * i));
		}
	}
	else if (type == e_center_relative)
	{
		SprPtr base = nullptr;
		float maxArea = 0;
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			sm::vec2 sz = m_sprs[i]->GetBounding().GetSize().Size();
			float area = sz.x * sz.y;
			if (area > maxArea)
			{
				maxArea = area;
				base = m_sprs[i];
			}
		}

		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
		{
			const SprPtr& spr = m_sprs[i];
			if (spr != base) {
				spr->SetPosition(base->GetPosition());
			}
		}
	}
}

void MultiSpritesPropertyImpl::OnPosChange(float dx, float dy)
{
	SetWndDirtySJ::Instance()->SetDirty();

	EditAddRecordSJ::Instance()->Add(std::make_shared<TranslateSpriteAOP>(m_sprs, sm::vec2(dx, dy)));
	for (int i = 0, n = m_sprs.size(); i < n; ++i)
	{
		const SprPtr& spr = m_sprs[i];
		spr->Translate(sm::vec2(dx, dy));
	}
}

void MultiSpritesPropertyImpl::OnAngleChange(float angle)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprs.size(); i < n; ++i)
	{
		const SprPtr& spr = m_sprs[i];
		spr->Rotate(angle);
	}
}

void MultiSpritesPropertyImpl::OnScaleChange(float dx, float dy)
{
	SetWndDirtySJ::Instance()->SetDirty();

	for (int i = 0, n = m_sprs.size(); i < n; ++i)
	{
		auto& s = m_sprs[i];
		sm::vec2 scale = s->GetScale();
		scale.x *= dx;
		scale.y *= dy;
		s->SetScale(scale);
	}
}

sm::vec2 MultiSpritesPropertyImpl::GetOverallCenter() const
{
	if (m_sprs.empty()) {
		return sm::vec2(0, 0);
	}

	SetWndDirtySJ::Instance()->SetDirty();

	sm::vec2 center(0, 0);
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		center += m_sprs[i]->GetPosition();
	}
	center /= m_sprs.size();
	return center;
}

}
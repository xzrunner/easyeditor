#include "RotateSpriteAOP.h"
#include "HistoryUtil.h"
#include "AtomicType.h"
#include "Math2D.h"
#include "Sprite.h"
#include "SpriteSelection.h"
#include "FetchAllVisitor.h"

#include <SM_Calc.h>

namespace ee
{

RotateSpriteAOP::RotateSpriteAOP(const std::vector<SprPtr>& sprs, const sm::vec2& start, const sm::vec2& end)
{
	Init(sprs, start, end);
}

RotateSpriteAOP::RotateSpriteAOP(const std::vector<SprPtr>& sprs, float angle)
{
	Init(sprs, angle);
}

RotateSpriteAOP::RotateSpriteAOP(const SpriteSelection& selection, const sm::vec2& start, const sm::vec2& end)
{
	std::vector<SprPtr> sprs;
	selection.Traverse(FetchAllRefVisitor<Sprite>(sprs));
	Init(sprs, start, end);
}

RotateSpriteAOP::RotateSpriteAOP(const SpriteSelection& selection, float angle)
{
	std::vector<SprPtr> sprs;
	selection.Traverse(FetchAllRefVisitor<Sprite>(sprs));
	Init(sprs, angle);
}

RotateSpriteAOP::RotateSpriteAOP(const SprPtr& spr, float angle)
{
	std::vector<SprPtr> sprs;
	sprs.push_back(spr);
	Init(sprs, angle);
}

void RotateSpriteAOP::Undo()
{
	if (m_inited) 
	{
		for (auto& spr : m_sprs) {
			float angle = sm::get_angle_in_direction(spr->GetPosition(), m_start, m_end);
			spr->Rotate(-angle);
		}
	} 
	else if (m_angle != 0) 
	{
		for (auto& spr : m_sprs) {
			spr->Rotate(-m_angle);
		}
	}
}

void RotateSpriteAOP::Redo()
{
	if (m_inited) 
	{
		for (auto& spr : m_sprs) {
			float angle = sm::get_angle_in_direction(spr->GetPosition(), m_start, m_end);
			spr->Rotate(angle);
		}
	} 
	else if (m_angle != 0) 
	{
		for (auto& spr : m_sprs) {
			spr->Rotate(m_angle);
		}
	}
}

Json::Value RotateSpriteAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprs, sprs);
	val["type"] = AT_ROTATE;
	val["xstart"] = m_start.x;
	val["ystart"] = m_start.y;
	val["xend"] = m_end.x;
	val["yend"] = m_end.y;
	val["angle"] = m_angle;
	return val;
}

void RotateSpriteAOP::Init(const std::vector<SprPtr>& sprs, 
						   const sm::vec2& start, const sm::vec2& end)
{
	m_inited = true;
	m_start = start;
	m_end = end;
	m_angle = 0;

	m_sprs = sprs;
}

void RotateSpriteAOP::Init(const std::vector<SprPtr>& sprs, float angle)
{
	m_inited = false;
	m_angle = angle;

	m_sprs = sprs;
}

}
#include "EE_SP.h"
#include "Sprite.h"
#include "BoundingBox.h"
#include "color_config.h"

#include <sprite2/S2_RVG.h>

#include <sp_null.h>
#include <sp_region.h>

namespace ee
{

SpatialPartition* SpatialPartition::m_instance = NULL;

static sp_null* SIDX;

SpatialPartition::SpatialPartition()
{
	SIDX = sp_null_create();
}

void SpatialPartition::Insert(const Sprite* spr/*, const sm::mat4& mt*/)
{
// 	sm::mat4 t;
// 	spr->GetTransMatrix(t);
//	t = t * mt;

	std::vector<sm::vec2> bound;
	spr->GetBounding()->GetBoundPos(bound);

	sp_region r;
	REGION_INIT(r);
	for (int i = 0, n = bound.size(); i < n; ++i) {
//		sm::vec2 p = Math::transVector(bound[i], t);
		const sm::vec2& p = bound[i];
		if (p.x < r.xmin) r.xmin = p.x;
		if (p.x > r.xmax) r.xmax = p.x;
		if (p.y < r.ymin) r.ymin = p.y;
		if (p.y > r.ymax) r.ymax = p.y;
	}

	sp_null_insert(SIDX, &r, (Sprite*)spr);
}

bool SpatialPartition::Remove(const Sprite* spr)
{
	return sp_null_remove(SIDX, (Sprite*)spr);
}

void SpatialPartition::Query(const sm::rect& region, std::vector<const Sprite*>& result) const
{
	sp_region r;
	r.xmin = region.xmin;
	r.xmax = region.xmax;
	r.ymin = region.ymin;
	r.ymax = region.ymax;

	static const int MAX_RETURN = 128;
	void* ud[MAX_RETURN];
	int return_count = 0;
	sp_null_query(SIDX, &r, ud, &return_count);
	for (int i = 0; i < return_count; ++i) {
		result.push_back((Sprite*)ud[i]);
	}
}

static void _render(float xmin, float ymin, float xmax, float ymax) 
{
	s2::RVG::SetColor(LIGHT_GREEN);
	s2::RVG::Rect(sm::vec2(xmin, ymin), sm::vec2(xmax, ymax), false);
}

void SpatialPartition::DebugDraw() const
{
	sp_null_debug_draw(SIDX, &_render);
}

SpatialPartition* SpatialPartition::Instance()
{
	if (!m_instance) {
		m_instance = new SpatialPartition();
	}
	return m_instance;
}

}
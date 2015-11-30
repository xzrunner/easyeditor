#include "SpatialIndex.h"

#include "common/Vector.h"
#include "common/Rect.h"
#include "common/Matrix.h"
#include "dataset/ISprite.h"
#include "dataset/AbstractBV.h"
#include "render/PrimitiveDraw.h"
#include "render/style_config.h"

#include <sidx_null.h>
#include <sidx_region.h>

namespace d2d
{

SpatialIndex* SpatialIndex::m_instance = NULL;

static sidx_null* SIDX;

SpatialIndex::SpatialIndex()
{
	SIDX = sidx_null_create();
}

void SpatialIndex::Insert(const ISprite* spr/*, const Matrix& mt*/)
{
// 	Matrix t;
// 	spr->GetTransMatrix(t);
//	t = mt * t;

	std::vector<Vector> bound;
	spr->GetBounding()->getBoundPos(bound);

	sidx_region r;
	REGION_INIT(r);
	for (int i = 0, n = bound.size(); i < n; ++i) {
//		Vector p = Math::transVector(bound[i], t);
		const Vector& p = bound[i];
		if (p.x < r.xmin) r.xmin = p.x;
		if (p.x > r.xmax) r.xmax = p.x;
		if (p.y < r.ymin) r.ymin = p.y;
		if (p.y > r.ymax) r.ymax = p.y;
	}

	sidx_null_insert(SIDX, &r, (ISprite*)spr);
}

bool SpatialIndex::Remove(const ISprite* spr)
{
	return sidx_null_remove(SIDX, (ISprite*)spr);
}

void SpatialIndex::Query(const Rect& region, std::vector<const ISprite*>& result) const
{
	sidx_region r;
	r.xmin = region.xMin;
	r.xmax = region.xMax;
	r.ymin = region.yMin;
	r.ymax = region.yMax;

	static const int MAX_RETURN = 128;
	void* ud[MAX_RETURN];
	int return_count = 0;
	sidx_null_query(SIDX, &r, ud, &return_count);
	for (int i = 0; i < return_count; ++i) {
		result.push_back((ISprite*)ud[i]);
	}
}

static void _render(float xmin, float ymin, float xmax, float ymax) 
{
	PrimitiveDraw::rect(Vector(xmin, ymin), Vector(xmax, ymax), LIGHT_GREEN_LINE);
}

void SpatialIndex::DebugDraw() const
{
	sidx_null_debug_draw(SIDX, &_render);
}

SpatialIndex* SpatialIndex::Instance()
{
	if (!m_instance) {
		m_instance = new SpatialIndex();
	}
	return m_instance;
}

}
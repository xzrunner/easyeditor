#include "SpatialIndex.h"

#include "common/Vector.h"
#include "common/Rect.h"
#include "common/Matrix.h"
#include "dataset/Sprite.h"
#include "dataset/BoundingBox.h"
#include "render/PrimitiveDraw.h"
#include "render/style_config.h"

#include <sidx_null.h>
#include <sidx_region.h>

namespace ee
{

SpatialIndex* SpatialIndex::m_instance = NULL;

static sidx_null* SIDX;

SpatialIndex::SpatialIndex()
{
	SIDX = sidx_null_create();
}

void SpatialIndex::Insert(const Sprite* spr/*, const Matrix& mt*/)
{
// 	Matrix t;
// 	spr->GetTransMatrix(t);
//	t = mt * t;

	std::vector<Vector> bound;
	spr->GetBounding()->GetBoundPos(bound);

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

	sidx_null_insert(SIDX, &r, (Sprite*)spr);
}

bool SpatialIndex::Remove(const Sprite* spr)
{
	return sidx_null_remove(SIDX, (Sprite*)spr);
}

void SpatialIndex::Query(const Rect& region, std::vector<const Sprite*>& result) const
{
	sidx_region r;
	r.xmin = region.xmin;
	r.xmax = region.xmax;
	r.ymin = region.ymin;
	r.ymax = region.ymax;

	static const int MAX_RETURN = 128;
	void* ud[MAX_RETURN];
	int return_count = 0;
	sidx_null_query(SIDX, &r, ud, &return_count);
	for (int i = 0; i < return_count; ++i) {
		result.push_back((Sprite*)ud[i]);
	}
}

static void _render(float xmin, float ymin, float xmax, float ymax) 
{
	PrimitiveDraw::DrawRect(Vector(xmin, ymin), Vector(xmax, ymax), LIGHT_GREEN_LINE);
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
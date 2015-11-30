#ifndef _DRAG2D_SPATIAL_INDEX_H_
#define _DRAG2D_SPATIAL_INDEX_H_

#include <vector>

namespace d2d
{

class ISprite;
class Matrix;
class Rect;

class SpatialIndex
{
public:
	void Insert(const ISprite* spr);
	bool Remove(const ISprite* spr);

	void Query(const Rect& region, std::vector<const ISprite*>& result) const;

	void DebugDraw() const;

	static SpatialIndex* Instance();

private:
	SpatialIndex();

private:
	static SpatialIndex* m_instance;

}; // SpatialIndex

}

#endif // _DRAG2D_SPATIAL_INDEX_H_
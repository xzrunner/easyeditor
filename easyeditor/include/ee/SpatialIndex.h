#ifndef _EASYEDITOR_SPATIAL_INDEX_H_
#define _EASYEDITOR_SPATIAL_INDEX_H_

#include <vector>

namespace ee
{

class Sprite;
class Matrix;
class Rect;

class SpatialIndex
{
public:
	void Insert(const Sprite* spr);
	bool Remove(const Sprite* spr);

	void Query(const Rect& region, std::vector<const Sprite*>& result) const;

	void DebugDraw() const;

	static SpatialIndex* Instance();

private:
	SpatialIndex();

private:
	static SpatialIndex* m_instance;

}; // SpatialIndex

}

#endif // _EASYEDITOR_SPATIAL_INDEX_H_
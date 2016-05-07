#ifndef _EASYEDITOR_SP_H_
#define _EASYEDITOR_SP_H_

#include <vector>

namespace ee
{

class Sprite;
class Rect;

class SpatialPartition
{
public:
	void Insert(const Sprite* spr);
	bool Remove(const Sprite* spr);

	void Query(const Rect& region, std::vector<const Sprite*>& result) const;

	void DebugDraw() const;

	static SpatialPartition* Instance();

private:
	SpatialPartition();

private:
	static SpatialPartition* m_instance;

}; // SpatialPartition

}

#endif // _EASYEDITOR_SP_H_
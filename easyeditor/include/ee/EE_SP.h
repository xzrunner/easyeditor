#ifndef _EASYEDITOR_SP_H_
#define _EASYEDITOR_SP_H_

#include <SM_Rect.h>

#include <vector>

namespace ee
{

class Sprite;

class SpatialPartition
{
public:
	void Insert(const Sprite* spr);
	bool Remove(const Sprite* spr);

	void Query(const sm::rect& region, std::vector<const Sprite*>& result) const;

	void DebugDraw() const;

	static SpatialPartition* Instance();

private:
	SpatialPartition();

private:
	static SpatialPartition* m_instance;

}; // SpatialPartition

}

#endif // _EASYEDITOR_SP_H_
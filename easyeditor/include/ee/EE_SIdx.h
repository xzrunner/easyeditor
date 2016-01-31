#ifndef _EASYEDITOR_SIDX_H_
#define _EASYEDITOR_SIDX_H_

#include <vector>

namespace ee
{

class Sprite;
class Matrix;
class Rect;

class SIdx
{
public:
	void Insert(const Sprite* spr);
	bool Remove(const Sprite* spr);

	void Query(const Rect& region, std::vector<const Sprite*>& result) const;

	void DebugDraw() const;

	static SIdx* Instance();

private:
	SIdx();

private:
	static SIdx* m_instance;

}; // SIdx

}

#endif // _EASYEDITOR_SIDX_H_
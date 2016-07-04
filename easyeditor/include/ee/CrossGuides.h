#ifndef _EASYEDITOR_CROSS_GUIDES_H_
#define _EASYEDITOR_CROSS_GUIDES_H_

#include <SM_Vector.h>

#include <vector>

namespace ee
{

class CrossGuides
{
public:
	CrossGuides();
	~CrossGuides();

	void Add();
	void Remove();

	bool OnMouseDown(const sm::vec2& pos);
	bool OnMouseUp(const sm::vec2& pos);
	bool OnMouseDrag(const sm::vec2& pos);

	void Draw() const;

private:
	struct Cross
	{
		static const int RADIUS = 10;
		static const int LENGTH = 100;

		Cross();
		void Draw() const;
		bool Contain(const sm::vec2& p) const;

		sm::vec2 pos;

	}; // Cross

private:
	std::vector<Cross*> m_crosses;

	Cross* m_selected;

}; // CrossGuides

}

#endif // _EASYEDITOR_CROSS_GUIDES_H_
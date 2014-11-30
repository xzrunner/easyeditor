#ifndef _EASYIMAGE_RECT_MANAGER_H_
#define _EASYIMAGE_RECT_MANAGER_H_

#include <drag2d.h>

#include "Rect.h"

namespace eimage
{

class RectPostProcessor
{
public:
	RectPostProcessor(const std::vector<Rect>& rects, int width, int height);
	~RectPostProcessor();

	void MoveToNoCover();
	
	void Merge();

	void LoadResult(std::vector<Rect>& rects) const;

private:
	struct Item;

	void LoadPixels(const std::vector<Rect>& rects);

	enum Direction
	{
		e_left,
		e_right,
		e_down,
		e_up
	};
	void MoveRect(Item* item, Direction dir);

private:
	struct Item
	{
		Item(const Rect& r) : r(r), is_static(false) {}

		Rect r;
		bool is_static;
	}; // Item

	class ItemCmp
	{
	public:
		bool operator () (const Item* lhs, const Item* rhs) const {
			return lhs->r.h * lhs->r.w < rhs->r.h * rhs->r.w;
		}
	}; // ItemCmp

	struct Pixel
	{
		void Add(Item* item) {
			items.insert(item);
		}
		void Remove(Item* item) {
			items.erase(item);
		}
		bool Find(Item* item) const {
			return items.find(item) != items.end();
		}
		bool IsCoverd() const {
			return items.size() > 1;
		}
		bool IsEmpty() const {
			return items.empty();
		}
		bool IsOnlyOneStatic() const {
			return items.size() == 1 && (*items.begin())->is_static;
		}

		std::set<Item*> items;
	}; // Pixel

private:
	int m_width, m_height;

	std::multiset<Item*, ItemCmp> m_items;
	Pixel** m_pixels;

}; // RectManager

}

#endif // _EASYIMAGE_RECT_MANAGER_H_
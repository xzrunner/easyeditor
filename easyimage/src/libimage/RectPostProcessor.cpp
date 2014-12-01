#include "RectPostProcessor.h"

namespace eimage
{

RectPostProcessor::RectPostProcessor(const std::vector<Rect>& rects, 
									 int width, int height,
									 bool* ori_pixels)
	: m_width(width)
	, m_height(height)
{
	LoadPixels(rects, ori_pixels);
}

RectPostProcessor::~RectPostProcessor()
{
	std::multiset<Item*, ItemCmp>::iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr) {
		delete *itr;
	}

	int sz = m_width * m_height;
	for (int i = 0; i < sz; ++i) {
		delete m_pixels[i];
	}
	delete[] m_pixels;
}

void RectPostProcessor::MoveToNoCover()
{
	std::multiset<Item*, ItemCmp>::const_iterator itr;
	for (itr = m_items.begin(); itr != m_items.end(); ++itr) {
		Item* item = *itr;

// 		bool move;
// 		do 
// 		{
// 			move = false;
// 			SetFreedom(item);
// 			if (item->to_right && MoveItem(item, e_right)) {
// 				move = true;
// 				continue;
// 			}
// 			if (item->to_left && MoveItem(item, e_left)) {
// 				move = true;
// 				continue;
// 			}
// 			if (item->to_up && MoveItem(item, e_up)) {
// 				move = true;
// 				continue;
// 			}
// 			if (item->to_down && MoveItem(item, e_down)) {
// 				move = true;
// 				continue;
// 			}
// 		} while (move);

		//////////////////////////////////////////////////////////////////////////

		SetFreedom(item);
		while (item->to_right && MoveItem(item, e_right)) {
			SetFreedom(item);
		}
		while (item->to_left && MoveItem(item, e_left)) {
			SetFreedom(item);
		}
		while (item->to_up && MoveItem(item, e_up)) {
			SetFreedom(item);
		}
		while (item->to_down && MoveItem(item, e_down)) {
			SetFreedom(item);
		}
	}
}

void RectPostProcessor::RemoveUnnecessary()
{
	std::multiset<Item*, ItemCmp>::const_iterator itr;
	for (itr = m_items.begin(); itr != m_items.end(); ) {
		Item* item = *itr;
		const Rect& r = item->r;

		bool remove = true;
		if (r.x < m_width && r.x+r.w > 0 &&
			r.y < m_height && r.y+r.h > 0) {
			for (int y = r.y, up = r.y + r.h; y < up && remove; ++y) {
				for (int x = r.x, right = r.x + r.w; x < right && remove; ++x) {
					Pixel* pixel = m_pixels[y*m_width+x];
					if (x >= 0 && x < m_width && y >= 0 && y < m_height &&
						pixel->HasData() && !pixel->IsCoverd()) {
						remove = false;
					}
				}
			}
		}

		if (remove) {
			itr = m_items.erase(itr);
		} else {
			++itr;
		}
	}
}

void RectPostProcessor::Merge()
{
	
}

void RectPostProcessor::LoadResult(std::vector<Rect>& rects) const
{
	rects.clear();
	std::multiset<Item*, ItemCmp>::const_iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr) {
		rects.push_back((*itr)->r);
	}
}

void RectPostProcessor::LoadPixels(const std::vector<Rect>& rects, bool* ori_pixels)
{
	int sz = m_width * m_height;
	m_pixels = new Pixel*[sz];
	for (int i = 0; i < sz; ++i) {
		m_pixels[i] = new Pixel(ori_pixels[i]);
	}

	for (int i = 0, n = rects.size(); i < n; ++i)
	{
		const Rect& r = rects[i];
		Item* item = new Item(r);
		m_items.insert(item);
		for (int y = r.y; y < r.y + r.h; ++y) {
			if (y < 0) { continue; } 
			else if (y >= m_height) { break; }
			for (int x = r.x; x < r.x + r.w; ++x) {
				if (x < 0) { continue; } 
				else if (x >= m_width) { break; }
				m_pixels[y*m_width+x]->Add(item);
			}
		}
	}
}

void RectPostProcessor::SetFreedom(Item* item)
{
	const Rect& r = item->r;

	int left = r.x, right = r.x+r.w-1;
	int down = r.y, up = r.y+r.h-1;
	int left_next = left-1, right_next = right+1;
	int down_next = down-1, up_next = up+1;

	// left
	item->to_left = true;
	if (right < 0) {
		item->to_left = false;
	} else if (right < m_width) {
		for (int y = down; y <= up; ++y) {
			const Pixel* pixel = m_pixels[y*m_width+right];
			if (y >= 0 && y < m_height && pixel->HasData() && !pixel->IsCoverd()) {
				item->to_left = false;
				break;
			}
		}
	}
	// right
	item->to_right = true;
	if (left >= m_width) {
		item->to_right = false;
	} else if (left >= 0) {
		for (int y = down; y <= up; ++y) {
			const Pixel* pixel = m_pixels[y*m_width+left];
			if (y >= 0 && y < m_height && pixel->HasData() && !pixel->IsCoverd()) {
				item->to_right = false;
				break;
			}
		}
	}
	// down
	item->to_down = true;
	if (up < 0) {
		item->to_down = false;
	} else if (up < m_height) {
		for (int x = left; x <= right; ++x) {
			const Pixel* pixel = m_pixels[up*m_width+x];
			if (x >= 0 && x < m_width && pixel->HasData() && !pixel->IsCoverd()) {
				item->to_down = false;
				break;
			}
		}
	}
	// up
	item->to_up = true;
	if (down >= m_height) {
		item->to_up = false;
	} else if (down >= 0) {
		for (int x = left; x <= right; ++x) {
			const Pixel* pixel = m_pixels[down*m_width+x];
			if (x >= 0 && x < m_width && pixel->HasData() && !pixel->IsCoverd()) {
				item->to_up = false;
				break;
			}
		}
	}
}

bool RectPostProcessor::MoveItem(Item* item, Direction dir)
{
	Rect& r = item->r;

	int left = r.x, right = r.x+r.w-1;
	int down = r.y, up = r.y+r.h-1;
	int left_next = left-1, right_next = right+1;
	int down_next = down-1, up_next = up+1;

#ifdef _DEBUG
	// not outside the region
	if (dir == e_left || dir == e_right) {
		assert(left < m_width || right >= 0);
	} else if (dir == e_down || dir == e_up) {
		assert(down < m_height || up >= 0);
	}
#endif

	bool move = true;
// 	// test src edge is covered
// 	if (dir == e_right && left >= 0 ||
// 		dir == e_left && right < m_width) 
// 	{
// 		int offset = (dir == e_right ? left : right);
// 		for (int y = down; y <= up; ++y) {
// 			if (y >= 0 && y < m_height && !m_pixels[y*m_width+offset]->IsCoverd()) {
// 				move = false;
// 				break;
// 			}
// 		}
// 	} 
// 	else if (dir == e_up && down >= 0 
// 		|| dir == e_down && up < m_height)
// 	{
// 		int offset = (dir == e_up ? down : up);
// 		for (int x = left; x <= right; ++x) {
// 			if (x >= 0 && x < m_width && !m_pixels[offset*m_width+x]->IsCoverd()) {
// 				move = false;
// 				break;
// 			}
// 		}
// 	}
// 	if (!move) {
// 		return false;
// 	}
	// test dst edge if not other one or which is not static
	move = true;
	if (dir == e_right && right_next < m_width ||
		dir == e_left && left_next >= 0)
	{
		int offset = (dir == e_right ? right_next : left_next);
		bool all_full = true;
		for (int y = down; y <= up; ++y) {
			const Pixel* pixel = m_pixels[y*m_width+offset];
			if (y >= 0 && y < m_height && pixel->IsEmpty()) {
				all_full = false;
				break;
			}
		}
		if (all_full) {
			move = false;
		}
	}
	else if (dir == e_up && up_next < m_height
		|| dir == e_down && down_next >= 0)
	{
		int offset = (dir == e_up ? up_next : down_next);
		bool all_full = true;
		for (int x = r.x; x <= right; ++x) {
			const Pixel* pixel = m_pixels[offset*m_width+x];
			if (x >= 0 && x < m_width && pixel->IsEmpty()) {
				all_full = false;
				break;
			}
		}
		if (all_full) {
			move = false;
		}
	}
	if (!move) {
		return false;
	}

	// remove from src
	if (dir == e_right && left >= 0 && left < m_width || 
		dir == e_left && right >= 0 && right < m_width)
	{
		int offset = (dir == e_right ? left : right);
		for (int y = down; y <= up; ++y) {
			if (y >= 0 && y < m_height) {
				Pixel* p = m_pixels[y*m_width+offset];
				assert(p->Find(item));
				p->Remove(item);
			}
		}
	}
	else if (dir == e_up && down >= 0 && down < m_height
		|| dir == e_down && up >= 0 && up < m_height)
	{
		int offset = (dir == e_up ? down : up);
		for (int x = left; x <= right; ++x) {
			if (x >= 0 && x < m_width) {
				Pixel* p = m_pixels[offset*m_width+x];
				assert(p->Find(item));
				p->Remove(item);
			}
		}
	}
	// add to dest
	if (dir == e_right && right_next >= 0 && right_next < m_width ||
		dir == e_left && left_next >= 0 && left_next < m_width)
	{
		int offset = (dir == e_right ? right_next : left_next);
		for (int y = down; y <= up; ++y) {
			if (y >= 0 && y < m_height) {
				Pixel* p = m_pixels[y*m_width+offset];
				p->Add(item);
			}
		}				
	}
	else if (dir == e_up && up_next >= 0 && up_next < m_height
		|| dir == e_down && down_next >= 0 && down_next < m_height)
	{
		int offset = (dir == e_up ? up_next : down_next);
		for (int x = left; x <= right; ++x) {
			if (x >= 0 && x < m_width) {
				Pixel* p = m_pixels[offset*m_width+x];
				p->Add(item);
			}
		}
	}

	if (dir == e_right) { ++r.x; }
	else if (dir == e_left) { --r.x; }
	else if (dir == e_up) { ++r.y; }
	else { --r.y; }

	if (left >= m_width || right < 0 || down >= m_height || up < 0) {
		return false;
	} else {
		return true;
	}
}

}
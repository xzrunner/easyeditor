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
	for (itr = m_items.begin(); itr != m_items.end(); ++itr) 
	{
		Item* item = *itr;

		UpdateFreedom(item);

		bool left_empty = IsNoCoverdSide(item, e_left);
		bool right_empty = IsNoCoverdSide(item, e_right);
		if (left_empty && !right_empty) {
			while (item->to_left && MoveItem(item, e_left)) {
				UpdateFreedom(item);
			}
		} else {
			while (item->to_right && MoveItem(item, e_right)) {
				UpdateFreedom(item);
			}
		}

		bool down_empty = IsNoCoverdSide(item, e_down);
		bool up_empty = IsNoCoverdSide(item, e_up);
		if (down_empty && !up_empty) {
			while (item->to_down && MoveItem(item, e_down)) {
				UpdateFreedom(item);
			}
		} else {
			while (item->to_up && MoveItem(item, e_up)) {
				UpdateFreedom(item);
			}
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
					if (IsPixelImmoveable(x, y)) {
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

bool RectPostProcessor::Merge()
{
	bool dirty = false;

	std::multiset<Item*, ItemCmp>::const_iterator itr;
	for (itr = m_items.begin(); itr != m_items.end(); ) 
	{
		Item* item = *itr;
		const Rect& r = item->r;
		// left
		assert(r.x < m_width);
		if (r.x > 0) {
			bool find = false;
			for (int y = r.y, up = r.y + r.h; y < up; ++y) {
				if (y < 0 || y >= m_height) {
					continue;
				}
				Pixel* p = m_pixels[y*m_width+r.x-1];
				Item* new_item = p->FindSpecialRect(-1, r.y, -1, r.h);
				if (new_item) {
					find = true;
					MergeRect(item, new_item);
					itr = m_items.erase(itr);
				}
				break;
			}
			if (find) {
				dirty = true;
				continue;
			}
		}
		// down
		assert(r.y < m_height);
		if (r.y > 0) {
			bool find = false;
			for (int x = r.x, right = r.x + r.w; x < right; ++x) {
				if (x < 0 || x >= m_width) {
					continue;
				}
				Pixel* p = m_pixels[(r.y-1)*m_width+x];
				Item* new_item = p->FindSpecialRect(r.x, -1, r.w, -1);
				if (new_item) {
					find = true;
					MergeRect(item, new_item);
					itr = m_items.erase(itr);
				}
				break;
			}
			if (find) {
				dirty = true;
				continue;
			}
		}

		++itr;
	}

	return dirty;
}

void RectPostProcessor::Align()
{
	std::multiset<Item*, ItemCmp>::const_iterator itr;
	for (itr = m_items.begin(); itr != m_items.end(); ++itr) 
	{
		Item* item = *itr;

		if (item->r.x == 204 && item->r.y == 165) {
			int zz = 0;
		}

		UpdateFreedom(item);

		while (true) {
			if (IsAlignBetter(item, e_left) && MoveItem(item, e_left)) {
				UpdateFreedom(item);
			} else {
				break;
			}
		}
		while (true) {
			if (IsAlignBetter(item, e_right) && MoveItem(item, e_right)) {
				UpdateFreedom(item);
			} else {
				break;
			}
		}
		while (true) {
			if (IsAlignBetter(item, e_down) && MoveItem(item, e_down)) {
				UpdateFreedom(item);
			} else {
				break;
			}
		}
		while (true) {
			if (IsAlignBetter(item, e_up) && MoveItem(item, e_up)) {
				UpdateFreedom(item);
			} else {
				break;
			}
		}
	}
}

void RectPostProcessor::Reduce()
{
	const int LIMIT_AREA = 4;

	std::multiset<Item*, ItemCmp>::const_iterator itr;
	for (itr = m_items.begin(); itr != m_items.end(); ) 
	{
		Item* item = *itr;
		int area = GetItemDataSize(item);
		if (area >= LIMIT_AREA) {
			++itr;
			continue;
		}

		int enlarge_min = INT_MAX;
		Item* enlarge_item = NULL;

		std::multiset<Item*, ItemCmp>::const_iterator itr2;
		for (itr2 = m_items.begin(); itr2 != m_items.end(); ++itr2) 
		{
			if (itr == itr2) {
				continue;
			}
			Item* item2 = *itr2;
			int left = std::min(item->r.x, item2->r.x),
				right = std::max(item->r.x + item->r.w, item2->r.x + item2->r.w);
			int down = std::min(item->r.y, item2->r.y),
				up = std::max(item->r.y + item->r.h, item2->r.y + item2->r.h);
			int enlarge = (right - left) * (up - down) - item2->r.w * item2->r.h;
			if (enlarge < enlarge_min) {
				enlarge_min = enlarge;
				enlarge_item = item2;
			}
		}

		if (enlarge_item) {
			
			itr = m_items.erase(itr);
		} else {
			++itr;
		}
	}
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

void RectPostProcessor::UpdateFreedom(Item* item)
{
	const Rect& r = item->r;

	int left = r.x, right = r.x+r.w-1;
	int down = r.y, up = r.y+r.h-1;

	// left
	item->to_left = true;
	if (right < 0) {
		item->to_left = false;
	} else if (right < m_width) {
		for (int y = down; y <= up; ++y) {
			if (IsPixelImmoveable(right, y)) {
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
			if (IsPixelImmoveable(left, y)) {
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
			if (IsPixelImmoveable(x, up)) {
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
			if (IsPixelImmoveable(x, down)) {
				item->to_up = false;
				break;
			}
		}
	}
}

bool RectPostProcessor::IsNoCoverdSide(Item* item, Direction dir) const
{
	const Rect& r = item->r;

	int left = r.x, right = r.x+r.w-1;
	int down = r.y, up = r.y+r.h-1;
	int left_next = left-1, right_next = right+1;
	int down_next = down-1, up_next = up+1;

	bool ret = true;
	if (dir == e_left) {
		if (left_next >= 0 && left_next < m_width) {
			for (int y = down; y <= up; ++y) {
				if (IsPixelCovered(left_next, y)) {
					ret = false;
					break;
				}
			}
		}
	} else if (dir == e_right) {
		if (right_next >= 0 && right_next < m_width) {
			for (int y = down; y <= up; ++y) {
				if (IsPixelCovered(right_next, y)) {
					ret = false;
					break;
				}
			}
		}
	} else if (dir == e_down) {
		if (down_next >= 0 && down_next < m_height) {
			for (int x = left; x <= right; ++x) {
				if (IsPixelCovered(x, down_next)) {
					ret = false;
					break;
				}
			}
		}
	} else if (dir == e_up) {
		if (up_next >= 0 && up_next < m_height) {
			for (int x = left; x <= right; ++x) {
				if (IsPixelCovered(x, up_next)) {
					ret = false;
					break;
				}
			}
		}
	}
	return ret;
}

bool RectPostProcessor::IsAlignBetter(Item* item, Direction dir) const
{
	if (!IsNoCoverdSide(item, dir)) {
		return false;
	}

	bool ret = false;

	const Rect& r = item->r;

	int left = r.x, right = r.x+r.w-1;
	int down = r.y, up = r.y+r.h-1;
	int left_next = left-1, right_next = right+1;
	int down_next = down-1, up_next = up+1;
	
	if (dir == e_left && item->to_left) {
		int count = 0;
		for (int y = down; y <= up; ++y) {
			if (IsPixelCovered(left_next - 1, y)) {
				++count;
			}
		}
		if (IsPixelCovered(left_next, up_next)) {
			++count;
		}
		if (IsPixelCovered(left_next, down_next)) {
			++count;
		}

		for (int y = down; y <= up; ++y) {
			if (IsPixelCovered(right_next, y)) {
				--count;
			}
		}
		if (IsPixelCovered(right, up_next)) {
			--count;
		}
		if (IsPixelCovered(right, down_next)) {
			--count;
		}
		if (count > 0) {
			ret = true;
		}
	} else if (dir == e_right && item->to_right) {
		int count = 0;
		for (int y = down; y <= up; ++y) {
			if (IsPixelCovered(right_next + 1, y)) {
				++count;
			}
		}
		if (IsPixelCovered(right_next, up_next)) {
			++count;
		}
		if (IsPixelCovered(right_next, down_next)) {
			++count;
		}

		for (int y = down; y <= up; ++y) {
			if (IsPixelCovered(left_next, y)) {
				--count;
			}
		}
		if (IsPixelCovered(left, up_next)) {
			--count;
		}
		if (IsPixelCovered(left, down_next)) {
			--count;
		}
		if (count > 0) {
			ret = true;
		}
	} else if (dir == e_down && item->to_down) {
		int count = 0;
		for (int x = left; x <= right; ++x) {
			if (IsPixelCovered(x, down_next - 1)) {
				++count;
			}
		}
		if (IsPixelCovered(left_next, down_next)) {
			++count;
		}
		if (IsPixelCovered(right_next, down_next)) {
			++count;
		}

		for (int x = left; x <= right; ++x) {
			if (IsPixelCovered(x, up_next)) {
				--count;
			}
		}
		if (IsPixelCovered(left_next, up)) {
			--count;
		}
		if (IsPixelCovered(right_next, up)) {
			--count;
		}
		if (count > 0) {
			ret = true;
		}
	} else if (dir == e_up && item->to_up) {
		int count = 0;
		for (int x = left; x <= right; ++x) {
			if (IsPixelCovered(x, up_next + 1)) {
				++count;
			}
		}
		if (IsPixelCovered(left_next, up_next)) {
			++count;
		}
		if (IsPixelCovered(right_next, up_next)) {
			++count;
		}

		for (int x = left; x <= right; ++x) {
			if (IsPixelCovered(x, down_next)) {
				--count;
			}
		}
		if (IsPixelCovered(left_next, down)) {
			--count;
		}
		if (IsPixelCovered(right_next, down)) {
			--count;
		}
		if (count > 0) {
			ret = true;
		}
	}

	return ret;
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
			if (y >= 0 && y < m_height && 
				offset >= 0 && offset < m_width &&
				m_pixels[y*m_width+offset]->IsEmpty()) {
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
			if (x >= 0 && x < m_width && 
				offset >= 0 && offset < m_height &&
				m_pixels[offset*m_width+x]->IsEmpty()) {
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
			if (y >= 0 && y < m_height && offset >= 0 && offset < m_width) {
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
			if (x >= 0 && x < m_width && offset >= 0 && offset < m_height) {
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
			if (y >= 0 && y < m_height && offset >= 0 && offset < m_width) {
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
			if (x >= 0 && x < m_width && offset >= 0 && offset < m_height) {
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

void RectPostProcessor::MergeRect(Item* remove, Item* newone)
{
	const Rect& r = remove->r;
	for (int y = r.y, up = r.y+r.h; y < up; ++y) {
		for (int x = r.x, right = r.x+r.w; x < right; ++x) {
			if (x >= 0 && x < m_width &&
				y >= 0 && y < m_height)
			{
				Pixel* p = m_pixels[y*m_width+x];
				p->Remove(remove);
				p->Add(newone);
			}
		}
	}
	Rect& newr = newone->r;
	if (r.y == newr.y && r.h == newr.h) {
		int x = std::min(r.x, newr.x);
		newr.w = std::max(r.x+r.w, newr.x+newr.w) - x;
		// fix to times of 4
		if (newr.w % 4 != 0) {
			newr.w = 4 * std::ceil(newr.w / 4.0f);
		}
		newr.x = x;
	} else {
		assert(r.x == newr.x && r.w == newr.w);
		int y = std::min(r.y, newr.y);
		newr.h = std::max(r.y+r.h, newr.y+newr.h) - y;
		// fix to times of 4
		if (newr.h % 4 != 0) {
			newr.h = 4 * std::ceil(newr.h / 4.0f);
		}
		newr.y = y;
	}
}

// bool RectPostProcessor::PixelHasData(int x, int y) const
// {
// 	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
// 		return false;
// 	} else {
// 		return m_pixels[y * m_width + x]->HasData();
// 	}
// }

bool RectPostProcessor::IsPixelCovered(int x, int y) const
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		return false;
	} else {
		return !m_pixels[y * m_width + x]->IsEmpty();
	}
}

bool RectPostProcessor::IsPixelImmoveable(int x, int y) const
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		return false;
	} else {
		Pixel* p = m_pixels[y * m_width + x];
		return p->HasData() && !p->IsCoverd();
	}
}

int RectPostProcessor::GetItemDataSize(Item* item) const
{
	int size = 0;
	const Rect& r = item->r;
	for (int x = r.x; x < r.x + r.w; ++x) {
		if (x < 0) {
			continue;
		}
		if (x >= m_width) {
			break;
		}
		for (int y = r.y; y < r.y + r.h; ++y) {
			if (y < 0) {
				continue;
			}
			if (y >= m_height) {
				break;
			}
			if (m_pixels[y * m_width + x]->HasData()) {
				++size;
			}
		}
	}
	return size;
}

//////////////////////////////////////////////////////////////////////////
// class RectPostProcessor::Pixel
//////////////////////////////////////////////////////////////////////////

RectPostProcessor::Item* RectPostProcessor::Pixel::
FindSpecialRect(int x, int y, int w, int h) const
{
	std::set<Item*>::const_iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr) {
		const Rect& r = (*itr)->r;
		if (r.y == y && r.h == h ||
			r.x == x && r.w == w) {
			return (*itr);
		}
	}
	return NULL;
}

}
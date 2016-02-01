#include "RegularRectCondense.h"

#include <algorithm>

#include <assert.h>

namespace eimage
{

RegularRectCondense::RegularRectCondense(const std::vector<Rect>& rects, int width, int height, bool* ori_pixels)
	: m_width(width)
	, m_height(height)
{
	LoadPixels(rects, ori_pixels);
}

RegularRectCondense::~RegularRectCondense()
{
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		delete m_grids[i];
	}

	int sz = m_width * m_height;
	for (int i = 0; i < sz; ++i) {
		delete m_pixels[i];
	}
	delete[] m_pixels;
}

void RegularRectCondense::Condense(const Rect& r)
{
	std::sort(m_grids.begin(), m_grids.end(), AreaCmp());
	CondenseCovered(r);
}

void RegularRectCondense::Remove(const Rect& r)
{
	assert(r.x >= 0 && r.x < m_width 
		&& r.y >= 0 && r.y < m_height);
	Grid* g = m_pixels[r.y * m_width + r.x]->QueryGrid(r);
	assert(g->r == r);

	RemovePixelGrid(g->r, g);

	bool find = false;
	std::vector<Grid*>::iterator itr = m_grids.begin();
	for ( ; itr != m_grids.end(); ++itr) {
		if (*itr == g) {
			m_grids.erase(itr);
			find = true;
			break;
		}
	}
	assert(find);
	delete g;
}

void RegularRectCondense::Insert(const Rect& r)
{
	Grid* g = new Grid(r);
	m_grids.push_back(g);
	InsertPixelGrid(r, g);
}

void RegularRectCondense::GetSortedRects(std::vector<Rect>& rects) const
{
	rects.clear();
	rects.reserve(m_grids.size());
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		rects.push_back(m_grids[i]->r);
	}
}

void RegularRectCondense::LoadPixels(const std::vector<Rect>& rects, 
									 bool* ori_pixels)
{
	int sz = m_width * m_height;
	m_pixels = new Pixel*[sz];
	for (int i = 0; i < sz; ++i) {
		m_pixels[i] = new Pixel(ori_pixels[i]);
	}

	for (int i = 0, n = rects.size(); i < n; ++i)
	{
		const Rect& r = rects[i];
		Grid* grid = new Grid(r);
		m_grids.push_back(grid);
		for (int y = r.y; y < r.y + r.h; ++y) {
			if (y < 0) { continue; } 
			else if (y >= m_height) { break; }
			for (int x = r.x; x < r.x + r.w; ++x) {
				if (x < 0) { continue; } 
				else if (x >= m_width) { break; }
				m_pixels[y*m_width+x]->Insert(grid);
			}
		}
	}

	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		CondenseEmpty(m_grids[i]);
	}
}

void RegularRectCondense::CondenseEmpty(Grid* g)
{
	if (g->r.w == 0 || g->r.h == 0) {
		return;
	}

	// left
	int len = 0;
	Rect r = g->r;
	while (true) {
		int x = r.x;
		bool find_data = false;
		for (int y = r.y; y < r.y + r.h; ++y) {
			if (IsPixelImmoveable(x, y)) {
				find_data = true;
				break;
			}
		}
		if (find_data) {
			break;
		} else {
			++r.x;
			++len;
			if (r.x == g->r.x + g->r.w) {
				break;
			}
		}
	}
	if (len > 0) {
		RemoveSize(g, e_left, len);
		if (g->r.w == 0 || g->r.h == 0) {
			return;
		}
	}

	// right
	len = 0;
	r = g->r;
	while (true) {
		int x = r.x + r.w - 1;
		bool find_data = false;
		for (int y = r.y; y < r.y + r.h; ++y) {
			if (IsPixelImmoveable(x, y)) {
				find_data = true;
				break;
			}
		}
		if (find_data) {
			break;
		} else {
			--r.w;
			++len;
			if (r.w == 0) {
				break;
			}
		}
	}
	if (len > 0) {
		RemoveSize(g, e_right, len);
		if (g->r.w == 0 || g->r.h == 0) {
			return;
		}
	}

	// down
	len = 0;
	r = g->r;
	while (true) {
		int y = r.y;
		bool find_data = false;
		for (int x = r.x; x < r.x + r.w; ++x) {
			if (IsPixelImmoveable(x, y)) {
				find_data = true;
				break;
			}
		}
		if (find_data) {
			break;
		} else {
			++r.y;
			++len;
			if (r.y == g->r.y + g->r.h) {
				break;
			}
		}
	}
	if (len > 0) {
		RemoveSize(g, e_down, len);
		if (g->r.w == 0 || g->r.h == 0) {
			return;
		}
	}

	// up
	len = 0;
	r = g->r;
	while (true) {
		int y = r.y + r.h - 1;
		bool find_data = false;
		for (int x = r.x; x < r.x + r.w; ++x) {
			if (IsPixelImmoveable(x, y)) {
				find_data = true;
				break;
			}
		}
		if (find_data) {
			break;
		} else {
			--r.h;
			++len;
			if (r.h == 0) {
				break;
			}
		}
	}
	if (len > 0) {
		RemoveSize(g, e_up, len);
		if (g->r.w == 0 || g->r.h == 0) {
			return;
		}
	}
}

bool RegularRectCondense::IsPixelImmoveable(int x, int y) const
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		return false;
	} else {
		Pixel* p = m_pixels[y * m_width + x];
		return p->HasData() && p->IsSingleCoverd();
	}
}

void RegularRectCondense::CondenseCovered(const Rect& r)
{
	std::vector<Grid*> grids;
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		if (IsRectIntersect(r, m_grids[i]->r)) {
			grids.push_back(m_grids[i]);
		}
	}

	for (int i = 0, n = grids.size(); i < n; ++i) {
		if (grids[i]->r.w == 0 || grids[i]->r.h == 0) {
			continue;
		}
		for (int j = 0; j < n; ++j) {
			if (i == j) {
				continue;
			}
			if (grids[j]->r.w == 0 || grids[j]->r.h == 0) {
				continue;
			}
			if (CondenseCovered(grids[i], grids[j])) {
				CondenseEmpty(grids[i]);
			}
		}
	}

	std::vector<Grid*>::iterator itr = m_grids.begin();
	for ( ; itr != m_grids.end(); ) {
		Grid* g = *itr;
		if (g->r.w == 0 || g->r.h == 0) {
			itr = m_grids.erase(itr);
			delete g;
		} else {
			++itr;
		}
	}
}

bool RegularRectCondense::CondenseCovered(Grid* s, Grid* l)
{
	bool ret = false;

	const Rect& sr = s->r;
	const Rect& lr = l->r;

	// contain
	if (sr.x >= lr.x && (sr.x + sr.w <= lr.x + lr.w) &&
		sr.y >= lr.y && (sr.y + sr.h <= lr.y + lr.h)) 
	{
		RemovePixelGrid(sr, s);
		s->r.w = 0;
		s->r.h = 0;
		return true;
	}

	// left
	if ((sr.x < lr.x + lr.w) && (sr.x >= lr.x) &&
		(sr.y >= lr.y) && (sr.y + sr.h <= lr.y + lr.h)) {
		RemoveSize(s, e_left, lr.x + lr.w - sr.x);
		ret = true;
	}
	// right
	if ((sr.x + sr.w - 1 >= lr.x) && (sr.x + sr.w - 1 < lr.x + lr.w) &&
		(sr.y >= lr.y) && (sr.y + sr.h <= lr.y + lr.h)) {
		RemoveSize(s, e_right, sr.x + sr.w - lr.x);
		ret = true;
	}
	// down
	if ((sr.y < lr.y + lr.h) && (sr.y >= lr.y) &&
		(sr.x >= lr.x) && (sr.x + sr.w <= lr.x + lr.w)) {
		RemoveSize(s, e_down, lr.y + lr.h - sr.y);
		ret = true;
	}
	// up
	if ((sr.y + sr.h - 1 >= lr.y) && (sr.y + sr.h - 1 < lr.y + lr.h) &&
		(sr.x >= lr.x) && (sr.x + sr.w <= lr.x + lr.w)) {
		RemoveSize(s, e_up, sr.y + sr.h - lr.y);
		ret = true;
	}
	return ret;
}

void RegularRectCondense::RemoveSize(Grid* g, Direction dir, int len)
{
	assert(len > 0);

	Rect rm = g->r;
	Rect rn = g->r;
	if (dir == e_left) 
	{
		assert(len <= g->r.w);
		rm.w = len;
		rn.w -= len;
		rn.x += len;
	} 
	else if (dir == e_right) 
	{
		assert(len <= g->r.w);
		rm.w = len;
		rm.x = g->r.x + g->r.w - len;
		rn.w -= len;
	} 
	else if (dir == e_down) 
	{
		assert(len <= g->r.h);
		rm.h = len;
		rn.h -= len;
		rn.y += len;
	}
	else if (dir == e_up)
	{
		assert(len <= g->r.h);
		rm.h = len;
		rm.y = g->r.y + g->r.h - len;
		rn.h -= len;
	}
	
	RemovePixelGrid(rm, g);

	g->r = rn;
}

void RegularRectCondense::RemovePixelGrid(const Rect& r, Grid* g)
{
	for (int x = r.x; x < r.x + r.w; ++x) {
		for (int y = r.y; y < r.y + r.h; ++y) {
			if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
				Pixel* p = m_pixels[y * m_width + x];
				p->Remove(g);
			}
		}
	}
}

void RegularRectCondense::InsertPixelGrid(const Rect& r, Grid* g)
{
	for (int x = r.x; x < r.x + r.w; ++x) {
		for (int y = r.y; y < r.y + r.h; ++y) {
			if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
				Pixel* p = m_pixels[y * m_width + x];
				p->Insert(g);
			}
		}
	}
}

bool RegularRectCondense::IsRectIntersect(const Rect& r0, const Rect& r1) const
{
	if (r0.x + r0.w - 1 < r1.x || 
		r0.x > r1.x + r1.w - 1 ||
		r0.y + r0.h - 1 < r1.y ||
		r0.y > r1.y + r1.h - 1) {
		return false;
	} else {
		return true;
	}
}

//////////////////////////////////////////////////////////////////////////
// class RegularRectCondense::AreaCmp
//////////////////////////////////////////////////////////////////////////

bool RegularRectCondense::AreaCmp::
operator () (const Grid* g0, const Grid* g1) const 
{
	return g0->r.w * g0->r.h < g1->r.w * g1->r.h;
}

//////////////////////////////////////////////////////////////////////////
// class RegularRectCondense::Pixel
//////////////////////////////////////////////////////////////////////////

void RegularRectCondense::Pixel::
Insert(Grid* g)
{
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		if (m_grids[i] == g) {
			return;
		}
	}	
	m_grids.push_back(g);
}

void RegularRectCondense::Pixel::
Remove(Grid* g)
{
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		if (m_grids[i] == g) {
			m_grids.erase(m_grids.begin() + i);
			return;
		}
	}
	assert(0);
}

RegularRectCondense::Grid* RegularRectCondense::Pixel::
QueryGrid(const Rect& r) const
{
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		if (m_grids[i]->r == r) {
			return m_grids[i];
		}
	}
	assert(0);
	return NULL;
}

}
#include "DFont.h"

#include <assert.h>
#include <string>

namespace d2d
{

DFont::DFont(int width, int height)
{
	Init(width, height);
}

DFont::~DFont()
{
	delete[] m_buffer;
}

void DFont::Clear()
{
	delete[] m_buffer;
	Init(m_width, m_height);
}

const DFont::dfont_rect* 
DFont::LookUp(int character, int font_size, int color, int is_edge) const
{
	return m_hash.LookUp(character, font_size, color, is_edge, 
		const_cast<list_head*>(&m_time), m_version);
}

DFont::dfont_rect* 
DFont::Insert(int character, int font_size, int color, int is_edge, int width, int height)
{
	if (width > m_width)
		return NULL;

	(LookUp(character,font_size,color,is_edge))?(assert(0), 0):(0);

 	for (;;) {
 		struct font_line *line = FindLine(width, height);
 		if (line == NULL)
 			break;
  		struct Hash::Rect * hr = FindSpace(line, width);
  		if (hr) {
  			return InsertChar(character,font_size,color,is_edge,hr);
  		}
 	}
 	struct Hash::Rect * hr = ReleaseSpace(width, height);
 	if (hr) {
 		return InsertChar(character,font_size,color,is_edge,hr);
 	}
	return NULL;
}

DFont::dfont_rect* 
DFont::Remove(int character, int font_size, int color, int is_edge)
{
	int h = Hash::GetHashVal(character, font_size, color, is_edge);
	struct Hash::Rect* hr = m_hash.m_hash[h];
	if (hr == NULL) {
		return NULL;
	}
	if (hr->c == character && hr->font == font_size && hr->color == color && hr->is_edge == is_edge) {
		m_hash.m_hash[h] = hr->next_hash;
		list_del(&hr->time);
		AdjustSpace(hr);
		return &hr->rect;
	}
	struct Hash::Rect* last = hr;
	hr = hr->next_hash;
	while (hr) {
		if (character == hr->c && hr->font == font_size && hr->color == color && hr->is_edge == is_edge) {
			last->next_hash = hr->next_hash;
			list_del(&hr->time);
			AdjustSpace(hr);
			return &hr->rect;
		}
		last = hr;
		hr = hr->next_hash;
	}

	return NULL;
}

void DFont::Init(int width, int height)
{
	m_width = width;
	m_height = height;

	m_max_line = m_version = m_dirty_count = 0;

	INIT_LIST_HEAD(&m_time);

	int max_line = height / TINY_FONT;
	int max_char = max_line * width / TINY_FONT;
	m_line = new font_line[max_line];
	m_hash.Init(max_char);

	int sz = width * height * 4;
	m_buffer = new uint32_t[sz];
	memset(m_buffer, 0, sz);
}

struct DFont::font_line* 
	DFont::NewLine(int height)
{
	int start_line = 0;
	if (m_max_line > 0) {
		struct font_line * lastline = &m_line[m_max_line-1];
		start_line = lastline->start_line + lastline->height;
	}
	if (start_line + height > m_height)
		return NULL;
	int max = m_height / TINY_FONT;
	if (m_max_line >= max)
		return NULL;
	struct font_line * line = &m_line[m_max_line++];
	line->start_line = start_line;
	line->height = height;
	line->space = m_width;
	INIT_LIST_HEAD(&line->head);
	return line;
}

struct DFont::font_line* 
	DFont::FindLine(int width, int height)
{
	int i;
	for (i=0;i<m_max_line;i++) {
		struct font_line * line = &m_line[i];
		if (height == line->height && width <= line->space) {
			return line;
		}
	}
	return NewLine(height);
}

DFont::Hash::Rect* 
	DFont::FindSpace(struct font_line* line, int width)
{
	int start_pos = 0;
	struct Hash::Rect * hr;
	int max_space = 0;
	list_for_each_entry(hr, struct Hash::Rect, &line->head, next_char) {
		int space = hr->rect.x - start_pos;
		if (space >= width) {
			struct Hash::Rect *n = m_hash.NewNode();
			if (n == NULL)
				return NULL;
			n->line = line - m_line;
			n->rect.x = start_pos;
			n->rect.y = line->start_line;
			n->rect.w = width;
			n->rect.h = line->height;
			list_add_tail(&n->next_char, &hr->next_char);

			return n;
		}

		if (space > max_space) {
			max_space = space;
		}
		start_pos = hr->rect.x + hr->rect.w;
	}
	int space = m_width - start_pos;
	if (space < width) {
		if (space > max_space) {
			line->space = space;
		} else {
			line->space = max_space;
		}
		return NULL;
	}
	struct Hash::Rect *n = m_hash.NewNode();
	if (n == NULL)
		return NULL;
	n->line = line - m_line;
	n->rect.x = start_pos;
	n->rect.y = line->start_line;
	n->rect.w = width;
	n->rect.h = line->height;
	list_add_tail(&n->next_char, &line->head);

	return n;
}

void DFont::AdjustSpace(struct Hash::Rect* hr)
{
	struct font_line *line = &m_line[hr->line];
	if (hr->next_char.next == &line->head) {
		hr->rect.w = m_width - hr->rect.x;
	} else {
		struct Hash::Rect *next = list_entry(hr->next_char.next, struct Hash::Rect, next_char);
		hr->rect.w = next->rect.x - hr->rect.x;
	}

	if (hr->next_char.prev == &line->head) {
		hr->rect.w += hr->rect.x;
		hr->rect.x = 0;
	} else {
		struct Hash::Rect *prev = list_entry(hr->next_char.prev, struct Hash::Rect, next_char);
		int x = prev->rect.x + prev->rect.w;
		hr->rect.w += hr->rect.x - x;
		hr->rect.x = x;
	}
	if (hr->rect.w > line->space) {
		line->space = hr->rect.w;
	}
}

struct DFont::Hash::Rect* 
	DFont::ReleaseChar(int character, int font_size, int color, int is_edge)
{
	int h = m_hash.GetHashVal(character, font_size, color, is_edge);
	struct Hash::Rect *hr = m_hash.m_hash[h];
	if (hr->c == character && hr->font == font_size && hr->color == color && hr->is_edge == is_edge) {
		m_hash.m_hash[h] = hr->next_hash;
		list_del(&hr->time);
		AdjustSpace(hr);
		return hr;
	}
	struct Hash::Rect * last = hr;
	hr = hr->next_hash;
	while (hr) {
		if (character == hr->c && hr->font == font_size && hr->color == color && hr->is_edge == is_edge) {
			last->next_hash = hr->next_hash;
			list_del(&hr->time);
			AdjustSpace(hr);
			return hr;
		}
		last = hr;
		hr = hr->next_hash;
	}
	assert(0);
	return NULL;
}

struct DFont::Hash::Rect* 
	DFont::ReleaseSpace(int width, int height)
{
	struct Hash::Rect *hr, *tmp;
	list_for_each_entry_safe(hr, struct Hash::Rect, tmp, &m_time, time) {
		if (hr->version == m_version)
			continue;
		if (hr->rect.h != height) {
			continue;
		}
		struct Hash::Rect * ret = ReleaseChar(hr->c, hr->font, hr->color, hr->is_edge);
		int w = hr->rect.w;
		if (w >= width) {
			ret->rect.w = width;
			return ret;
		} else {
			list_del(&ret->next_char);
			ret->next_hash = m_hash.m_freelist;
			m_hash.m_freelist = ret;
		}
	}
	return NULL;
}

struct DFont::dfont_rect* 
	DFont::InsertChar(int character, int font_size, int color, int is_edge, struct Hash::Rect* hr)
{
	hr->c = character;
	hr->font = font_size;
	hr->color = color;
	hr->is_edge = is_edge;
	hr->version = m_version;
	list_add_tail(&hr->time, &m_time);
	int h = m_hash.GetHashVal(character, font_size, color, is_edge);
	hr->next_hash = m_hash.m_hash[h];
	m_hash.m_hash[h] = hr;
	++m_dirty_count;
	return &hr->rect;
}

void DFont::CopyBuffer(uint32_t* char_buffer, int x, int y, int w, int h)
{
	if (char_buffer == NULL) {
		return;
	}
	int i, j;
	uint32_t* p = m_buffer + m_width * y + x;
	for(i = 0; i < h; i++){
		for(j = 0; j < w; j++){
			p[j] = char_buffer[j+i*w];
		}
		p = p + m_width;
	}
}

uint32_t* DFont::GetBuffer()
{
	return m_buffer;
}

bool DFont::IsDirty() const
{
	return m_dirty_count != 0;
}

void DFont::Flush()
{
	m_dirty_count = 0;
	++m_version;
}

//////////////////////////////////////////////////////////////////////////
// class DFont::Hash
//////////////////////////////////////////////////////////////////////////

DFont::Hash::
Hash()
{
	m_freelist = NULL;
	memset(m_hash, 0, sizeof(m_hash));
}

DFont::Hash::
~Hash()
{
	delete[] m_freelist;
}

void DFont::Hash::
Init(int capacity)
{
	m_freelist = new Rect[capacity];
	int i;
	for (i=0;i<capacity;i++) {
		Rect* hr = &m_freelist[i];
		hr->next_hash = &m_freelist[i+1];
	}
	m_freelist[capacity-1].next_hash = NULL;
}

const DFont::dfont_rect* DFont::Hash::
LookUp(int character, int font_size, int color, int is_edge, 
	   list_head* time, int version) const
{
	int h = Hash::GetHashVal(character, font_size, color, is_edge);
	struct Rect* hr = m_hash[h];
	while (hr) {
		if (hr->c == character && hr->font == font_size && hr->color == color && hr->is_edge == is_edge) {
			list_move(&hr->time, time);
			hr->version = version;
			return &(hr->rect);
		}
		hr = hr->next_hash;
	}
	return NULL;
}

DFont::Hash::Rect* DFont::Hash::
NewNode()
{
	if (m_freelist == NULL)
		return NULL;
	struct Rect* ret = m_freelist;
	m_freelist = ret->next_hash;
	return ret;
}

int DFont::Hash::
GetHashVal(int character, int font_size, int color, int is_edge)
{
	if (is_edge != 0 && is_edge != 1) {
		return abs(character ^ font_size ^ color ^ is_edge) % HASH_SIZE;
	} else {
		return (character ^ (font_size * 97) ^ (unsigned int)color ^ (is_edge * 31)) % HASH_SIZE;
	}	
}

}
// from ejoy2d

#ifndef _DRAG2D_DFONT_H_
#define _DRAG2D_DFONT_H_

#include <stdint.h>
#include <list.h>

namespace d2d
{

class DFont
{
public:
	struct dfont_rect;

public:
	DFont(int width, int height);
	~DFont();
	
	void Clear();

	const dfont_rect* LookUp(int character, int font_size, int color, int is_edge) const;
	dfont_rect* Insert(int character, int font_size, int color, int is_edge, int width, int height);
	dfont_rect* Remove(int character, int font_size, int color, int is_edge);

	void CopyBuffer(uint32_t* char_buffer, int x, int y, int w, int h);
	uint32_t* GetBuffer();

	bool IsDirty() const;
	void Flush();

public:
	struct dfont_rect 
	{
		int x;
		int y;
		int w;
		int h;
		int bearingX;
		int bearingY;
		int advance;
		int metrics_height;
		int bitmap_height;

		dfont_rect() {}
	};

private:
	class Hash
	{
	public:
		struct Rect;

	public:
		Hash();
		~Hash();

		void Init(int capacity);

		const dfont_rect* LookUp(int character, int font_size, int color, int is_edge,
			list_head* time, int version) const;

		Rect* NewNode();

		static int GetHashVal(int character, int font_size, int color, int is_edge);

	public:
		struct Rect
		{
			struct Rect * next_hash;
			struct list_head next_char;
			struct list_head time;
			int version;
			int line;
			int c;
			int font;
			int color;
			int is_edge;
			struct dfont_rect rect;
		};

	private:
		static const int HASH_SIZE = 4096;

	private:
		Rect* m_freelist;

		Rect* m_hash[HASH_SIZE];

		friend class DFont;

	}; // Hash

	struct font_line 
	{
		int start_line;
		int height;
		int space;
		struct list_head head;
	};

private:
	void Init(int width, int height);

	struct font_line* NewLine(int height);
	struct font_line* FindLine(int width, int height);	
	Hash::Rect* FindSpace(struct font_line* line, int width);
	void AdjustSpace(struct Hash::Rect* hr);
	struct Hash::Rect* ReleaseChar(int character, int font_size, int color, int is_edge);
	struct Hash::Rect* ReleaseSpace(int width, int height);
	struct dfont_rect* InsertChar(int character, int font_size, int color, int is_edge, struct Hash::Rect* hr);

private:
	static const int TINY_FONT = 12;

private:
	int m_width;
	int m_height;
	int m_max_line;
	int m_version;
	int m_dirty_count;
	struct list_head m_time;
	struct font_line* m_line;
	Hash m_hash;
	uint32_t* m_buffer;

}; // DFont

}

#endif // _DRAG2D_DFONT_H_
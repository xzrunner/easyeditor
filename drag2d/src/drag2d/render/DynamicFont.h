#ifndef _DRAG2D_DYNAMIC_FONT_H_
#define _DRAG2D_DYNAMIC_FONT_H_

#include "interfaces.h"

#include "common/tools.h"
#include "common/Object.h"
#include "dataset/FTRender.h"

#include <map>

namespace d2d
{

typedef unsigned int GLuint;

class TPNode;
class Screen;

struct Glyph : public Object
{
	bool is_used;

	// only hori current
	int bearing_x;
	int bearing_y;
	int advande;

	int metrics_height;
	int width, height;

	TPNode* tpnode;

	uint32_t* buffer;

	Glyph() {
		is_used = false;
		bearing_x = bearing_y = advande = 0;
		metrics_height = 0;
		width = height = 0;
		tpnode = NULL;
		buffer = NULL;
	}

	~Glyph() {
		delete[] buffer;
	}
}; // Glyph

class DynamicFont
{
public:
	static DynamicFont* Instance();

	const Glyph* LookUp(int character, int font_size, int color, int is_edge);

	void LoadFontFile(const char* filename);

	int GetTextureID() const { return m_tex; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	int GetPadding() const { return m_padding; }

	void ReloadTexture();

	void DebugDraw(const Screen& screen) const;

private:
	DynamicFont();
	~DynamicFont();

	uint32_t* GenFTChar(int unicode, int font_size, int color, int is_edge, GlyphLayout& layout);

	void Clear();

private:
	class Hash
	{
	public:
		Hash();
		~Hash();

		Glyph* LookUp(int character, int font_size, int color, int is_edge);

		void Clear();

		void Traverse(IVisitor& visitor) const;

	public:
		struct Node
		{
			Node* next;

			int character;
			int font_size;
			int color;
			int is_edge;

			Glyph glyph;
		}; // Node

	private:
		static int GetHashVal(int character, int font_size, int color, int is_edge);

	private:
		static const int HASH_SIZE = 4651;

	private:
		int m_capacity;
		Node* m_freelist;
		Node* m_freenode_ptr;

		Node* m_hash[HASH_SIZE];

	}; // Hash

	class ReloadTextureVisitor : public IVisitor
	{
	public:
		ReloadTextureVisitor(int tex, int padding) : m_tex(tex), m_padding(padding) {}
		virtual void visit(Object* object, bool& bFetchNext);
	private:
		int m_tex;
		int m_padding;
	}; // ReloadTextureVisitor

private:
	static const int WIDTH, HEIGHT;
	static const int PADDING;
	static const int FONT_SIZE_COUNT = 256;

private:
	int m_width, m_height;
	int m_padding;

	GLuint m_tex;
	GLuint m_fbo;

	TPNode* m_root;

	Hash m_hash;

	FTRender m_ft_render;
	GlyphSizer m_space_sizer[FONT_SIZE_COUNT];

private:
	static DynamicFont* m_instance;

}; // DynamicFont

}

#endif // _DRAG2D_DYNAMIC_FONT_H_
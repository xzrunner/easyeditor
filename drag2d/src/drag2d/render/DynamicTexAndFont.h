#ifndef _DRAG2D_DYNAMIC_TEXTURE_AND_FONT_H_
#define _DRAG2D_DYNAMIC_TEXTURE_AND_FONT_H_

#include "DynamicPacker.h"
#include "interfaces.h"

#include "common/tools.h"
#include "common/Rect.h"
#include "dataset/Image.h"
#include "dataset/FTRender.h"

#include <vector>

namespace d2d
{

typedef unsigned int GLuint;

class TPNode;
class Image;

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

class DynamicTexAndFont : public DynamicPacker
{
public:
	static DynamicTexAndFont* Instance();

	void Begin();
	void Insert(Image* img);
	void End();

	void Remove(Image* img);

	const TPNode* Query(const Image& img) const;

	const Glyph* LookUp(int character, int font_size, int color, int is_edge);

	void LoadFontFile(const char* filename);

protected:
	virtual void ReloadPixels();

private:
	DynamicTexAndFont();

	void InsertImage(const Image* img);

	void DrawNode(const TPNode* node, const Image* image) const;
	void DrawRegion(const Rect& vertex, const Rect& texcoords, int texid, bool is_rotate) const;
	void DrawExtrude(const Image* img, const TPNode* node) const;

	uint32_t* GenFTChar(int unicode, int font_size, int color, int is_edge, GlyphLayout& layout);

	void Clear();

private:
	class ImageSizeCmp
	{
	public:
		bool operator () (const Image* lhs, const Image* rhs) const {
			float short1, long1, short2, long2;
			GetEdgeLimit(lhs, short1, long1);
			GetEdgeLimit(rhs, short2, long2);
			if (long1 > long2) {
				return true;
			} else if (long1 < long2) {
				return false;
			} else {
				return short1 > short2;
			}
		}

	private:
		void GetEdgeLimit(const Image* img, float& max, float& min) const {
			Rect r = img->getRegion();
			if (r.xLength() > r.yLength()) {
				min = r.xLength();
				max = r.yLength();
			} else {
				min = r.yLength();
				max = r.xLength();
			}
		}
	}; // NodeCmp

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
	static const int PADDING, EXTRUDE;
	static const int FONT_SIZE_COUNT = 256;

private:
	// for image
	int m_extrude;

	int m_preload_idx;
	std::vector<const Image*> m_preload_list;

	std::map<wxString, TPNode*> m_map_images;
	
	// for font
	Hash m_hash;

	FTRender m_ft_render;
	GlyphSizer m_space_sizer[FONT_SIZE_COUNT];

private:
	static DynamicTexAndFont* m_instance;

}; // DynamicTexAndFont

}

#endif // _DRAG2D_DYNAMIC_TEXTURE_AND_FONT_H_
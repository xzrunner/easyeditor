#ifndef _DRAG2D_DYNAMIC_TEXTURE_H_
#define _DRAG2D_DYNAMIC_TEXTURE_H_

#include "common/Rect.h"
#include "dataset/Image.h"

#include <set>
#include <vector>

namespace d2d
{

typedef unsigned int GLuint;

class TPNode;
class Image;

class DynamicTexture
{
public:
	static DynamicTexture* Instance();

	void Begin();
	void Insert(Image* img);
	void End();

	float GetPadding() const { return m_padding; }

	const TPNode* Query(const Image& img) const;

	int GetTextureID() const { return m_tex; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	void ReloadTexture();

	void DebugDraw() const;

private:
	DynamicTexture();
	~DynamicTexture();

	void InitTexture(int width, int height, int tex_id = 0);
	void InitFBO(int fbo_id = 0);
	void InitRoot(int width, int height);

	void InsertImage(const Image* img);

	void DrawNode(const TPNode* node, const Image* image) const;
	void DrawRegion(const Rect& vertex, const Rect& texcoords, int texid, bool is_rotate) const;
	void DrawExtrude(const Image* img, const TPNode* node) const;

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

private:
	static const int WIDTH, HEIGHT;
	static const int PADDING, EXTRUDE;

private:
	int m_width, m_height;
	int m_padding, m_extrude;

	GLuint m_tex;
	GLuint m_fbo;

	TPNode* m_root;

	int m_preload_idx;
	std::vector<const Image*> m_preload_list;

	std::map<wxString, TPNode*> m_map_images;
	
private:
	static DynamicTexture* m_instance;

}; // DynamicTexture

}

#endif // _DRAG2D_DYNAMIC_TEXTURE_H_
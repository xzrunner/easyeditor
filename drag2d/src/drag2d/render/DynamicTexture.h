#ifndef _DRAG2D_DYNAMIC_TEXTURE_H_
#define _DRAG2D_DYNAMIC_TEXTURE_H_

#include "DynamicPacker.h"

#include "common/Rect.h"
#include "dataset/Image.h"

#include <vector>

namespace d2d
{

typedef unsigned int GLuint;

class TPNode;
class Image;

class DynamicTexture : public DynamicPacker
{
public:
	static DynamicTexture* Instance();

	void Begin();
	void Insert(Image* img);
	void End();

	void Remove(Image* img);

	const TPNode* Query(const Image& img) const;

protected:
	virtual void ReloadPixels();

private:
	DynamicTexture();

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
			min = img->GetClippedWidth();
			max = img->GetClippedHeight();
			if (min > max) {
				std::swap(min, max);
			}
		}
	}; // NodeCmp

private:
	static const int WIDTH, HEIGHT;
	static const int PADDING, EXTRUDE;

private:
	int m_extrude;

	int m_preload_idx;
	std::vector<Image*> m_preload_list;

	std::map<std::string, TPNode*> m_map_images;
	
private:
	static DynamicTexture* m_instance;

}; // DynamicTexture

}

#endif // _DRAG2D_DYNAMIC_TEXTURE_H_
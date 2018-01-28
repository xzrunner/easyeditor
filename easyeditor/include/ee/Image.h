#ifndef _EASYEDITOR_IMAGE_H_
#define _EASYEDITOR_IMAGE_H_

#include "ResourcesMgr.h"

#include <SM_Matrix.h>
#include <SM_Rect.h>
#include <cu/CU_RefCountObj.h>
#include <sprite2/pre_defined.h>
#include S2_MAT_HEADER

#include <stdint.h>

namespace pt2 { class Texture; }
namespace s2 { class Sprite; class RenderParams; class RenderTarget; }

namespace ee
{

class Sprite;
class Texture;

class Image
{
public:
	Image();
	Image(const uint8_t* pixels, int w, int h, int fmt);
	Image(const std::shared_ptr<const s2::RenderTarget>& rt);
	~Image();
	
	bool LoadFromFile(const std::string& filepath);

	const std::string& GetFilepath() const { return m_filepath; }

	int GetFormat() const;
	unsigned int GetTexID() const;

	const sm::i16_vec2& GetOriginSize() const { return m_ori_sz; }
	const sm::i16_rect& GetClippedRegion() const { return m_clipped_region; }

	void InvalidRect(const S2_MAT& mt) const;

	void QueryTexcoords(float* texcoords, int* texid) const;

	const std::shared_ptr<pt2::Texture>& GetS2Tex() const { return m_s2_tex; }

private:
	void LoadWithClip(const std::string& filepath);

	bool CanUseDTex() const;

private:
	std::string m_filepath;

	std::shared_ptr<Texture> m_tex;
	std::shared_ptr<pt2::Texture> m_s2_tex;

	sm::i16_vec2 m_ori_sz;
	sm::i16_rect m_clipped_region;

}; // Image

typedef ResourcesMgr<Image> ImageMgr;

using ImagePtr = std::shared_ptr<Image>;
using ImageConstPtr = std::shared_ptr<const Image>;

}

#endif // _EASYEDITOR_IMAGE_H_
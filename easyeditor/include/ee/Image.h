#ifndef _EASYEDITOR_IMAGE_H_
#define _EASYEDITOR_IMAGE_H_

#include "ResourcesMgr.h"

#include <SM_Matrix.h>
#include <SM_Rect.h>
#include <CU_RefCountObj.h>

#include <stdint.h>

namespace s2 { class Sprite; class RenderParams; class Texture; class RenderTarget; }

namespace ee
{

class Sprite;
class Texture;

class Image : public cu::RefCountObj
{
public:
	Image();
	Image(const uint8_t* pixels, int w, int h, int fmt);
	Image(const s2::RenderTarget* rt);
	~Image();
	
	bool LoadFromFile(const std::string& filepath);

	const std::string& GetFilepath() const { return m_filepath; }

	int GetFormat() const;
	unsigned int GetTexID() const;

	const sm::i16_vec2& GetOriginSize() const { return m_ori_sz; }
	const sm::i16_rect& GetClippedRegion() const { return m_clipped_region; }

	void InvalidRect(const sm::mat4& mt) const;

	void QueryTexcoords(float* texcoords, int* texid) const;

	s2::Texture* GetS2Tex() const { return m_s2_tex; }

private:
	void LoadWithClip(const std::string& filepath);

	bool CanUseDTex() const;

private:
	std::string m_filepath;

	Texture* m_tex;
	s2::Texture* m_s2_tex;

	sm::i16_vec2 m_ori_sz;
	sm::i16_rect m_clipped_region;

}; // Image

typedef ResourcesMgr<Image> ImageMgr;

}

#endif // _EASYEDITOR_IMAGE_H_
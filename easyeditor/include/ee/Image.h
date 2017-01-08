#ifndef _EASYEDITOR_IMAGE_H_
#define _EASYEDITOR_IMAGE_H_

#include "ResourcesMgr.h"

#include <SM_Matrix.h>
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

	std::string GetFilepath() const;
	unsigned int GetTexID() const;
	int GetOriginWidth() const;
	int GetOriginHeight() const;
	int GetClippedWidth() const;
	int GetClippedHeight() const;

	void InvalidRect(const sm::mat4& mt) const;

	const sm::vec2& GetOffset() const { return m_offset; }

	void QueryTexcoords(float* texcoords, int* texid) const;

	s2::Texture* GetS2Tex() const { return m_s2_tex; }

private:
	void LoadWithClip(const std::string& filepath);

	bool CanUseDTex() const;

private:
	Texture* m_tex;
	s2::Texture* m_s2_tex;

	float m_ori_w, m_ori_h;

	sm::vec2 m_offset;

	int m_xmin, m_ymin;

}; // Image

typedef ResourcesMgr<Image> ImageMgr;

}

#endif // _EASYEDITOR_IMAGE_H_
#ifndef _GLUE_IMAGE_H_
#define _GLUE_IMAGE_H_

#include "Resource.h"
#include "ResourceManager.h"

#include <sprite2/Texture.h>

#include <SM_Vector.h>

#include <stdint.h>

namespace glue
{

class Image;
typedef ResourceManager<Image> ImageMgr;

class Image : public Resource
{
public:
	Image();
	virtual ~Image();

	virtual bool LoadFromFile(const std::string& filepath);

	sm::ivec2 GetSize() const {
		return sm::ivec2(m_width, m_height);
	}

	uint32_t GetTexID() const { return m_id; }

	s2::Texture* GetS2Tex() const { return m_s2_tex; }

	const std::string& GetFilepath() const { return m_filepath; }

private:
	void Load();

private:
	std::string m_filepath;

	int m_width, m_height;
	int m_format;

	uint32_t m_id;

	s2::Texture* m_s2_tex;

}; // Image

}

#endif // _GLUE_IMAGE_H_
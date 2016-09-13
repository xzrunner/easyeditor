#ifndef _GUM_IMAGE_H_
#define _GUM_IMAGE_H_

#include "Resource.h"
#include "ResourceManager.h"

#include <sprite2/Texture.h>
#include <simp/FileLoader.h>

#include <SM_Vector.h>

#include <stdint.h>

namespace gum
{

class Image;
typedef ResourceManager<Image> ImageMgr;

class Image : public Resource
{
public:
	Image();
	virtual ~Image();

	virtual bool LoadFromFile(const std::string& filepath);

	void LoadBin(const std::string& filepath);

	sm::ivec2 GetSize() const {
		return sm::ivec2(m_width, m_height);
	}

	uint32_t GetTexID() const { return m_id; }

	s2::Texture* GetS2Tex() const { return m_s2_tex; }

	const std::string& GetFilepath() const { return m_filepath; }

private:
	class Loader : public simp::FileLoader
	{
	public:
		Loader(const std::string& filepath, Image* img);
	protected:
		virtual void OnLoad(simp::ImportStream& is);
	private:
		Image* m_img;
	}; // Loader

private:
	std::string m_filepath;

	int m_width, m_height;
	int m_format;

	uint32_t m_id;

	s2::Texture* m_s2_tex;

}; // Image

}

#endif // _GUM_IMAGE_H_

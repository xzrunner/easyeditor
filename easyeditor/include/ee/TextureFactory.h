#ifndef _EASYEDITOR_TEXTURE_FACTORY_H_
#define _EASYEDITOR_TEXTURE_FACTORY_H_

#include <SM_Vector.h>

#include <string>

#include <stdint.h>

namespace ee
{

class TexturePacker;

class TextureFactory
{
public:
	void InitTexturePacker(const std::string& src_data_dir);
	void AddTextureFromConfig(const std::string& filepath);

	const uint8_t* Load(const std::string& filepath, int& width, 
		int& height, int& format) const;
	void Load(const std::string& filepath, float& ori_w, float& ori_h, 
		int& w, int& h, sm::vec2& offset) const;

	static TextureFactory* Instance();

private:
	TextureFactory();

private:
	TexturePacker* m_tp;

	static TextureFactory* m_instance;

}; // TextureFactory

}

#endif // _EASYEDITOR_TEXTURE_FACTORY_H_
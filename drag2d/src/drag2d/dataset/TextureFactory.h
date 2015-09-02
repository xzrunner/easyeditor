#ifndef _DRAG2D_TEXTURE_MGR_H_
#define _DRAG2D_TEXTURE_MGR_H_

#include <string>
#include <stdint.h>

#include "common/Vector.h"

namespace d2d
{

class TexturePacker;

class TextureFactory
{
public:

	void InitTexturePacker(const std::string& src_data_dir);
	void AddTextureFromConfig(const std::string& filepath);

	const uint8_t* Load(const std::string& filepath, 
		int& width, int& height, int& channels, int& format) const;
	void Load(const std::string& filepath,
		float& ori_w, float& ori_h, Vector& offset) const;

	static TextureFactory* Instance();

private:
	TextureFactory();

private:
	TexturePacker* m_tp;

	static TextureFactory* m_instance;

}; // TextureFactory

}

#endif // _DRAG2D_TEXTURE_MGR_H_
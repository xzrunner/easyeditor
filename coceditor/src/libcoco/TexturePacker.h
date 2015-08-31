#ifndef _LIBCOCO_TEXTURE_PACKER_H_
#define _LIBCOCO_TEXTURE_PACKER_H_

#include <drag2d.h>

namespace libcoco
{

class TexturePacker
{
public:
	TexturePacker(const std::string& src_data_dir);
	~TexturePacker();
	
	void Add(const std::string& filepath);

	struct Frame;
	const Frame* Query(const std::string& filepath) const;
	
public:
	struct Region
	{
		int x, y;
		int w, h;

		void Load(const Json::Value& value);
	};

	struct FrameSrcData
	{
		std::string filename;

		Region frame;

		bool rotated;

		bool trimmed;
		Region sprite_source_size;

		int src_width, src_height;

		void Load(const Json::Value& value, const std::string& src_data_dir);
	};

	struct Texture;
	struct FrameDstData
	{
		// 0 3
		// 1 2
		d2d::Vector tex_coords[4];

		d2d::Vector offset;

		void Load(const FrameSrcData& src, const Texture* tex);
	};

	struct Frame
	{
		FrameSrcData src;
		FrameDstData dst;

		int tex_idx;

		void Load(const Json::Value& value, const std::string& src_data_dir,
			const Texture* tex);
	};

	struct Texture
	{
		int idx;

		int width, height;

		std::map<std::string, Frame> frames;

		bool is_easydb;

		void Load(const Json::Value& value, const std::string& src_data_dir);

		const Frame* Query(const std::string& filepath) const;
	};

private:
	std::string m_src_data_dir;

	std::vector<Texture*> m_textures;

}; // TexturePacker

}

#endif // _LIBCOCO_TEXTURE_PACKER_H_
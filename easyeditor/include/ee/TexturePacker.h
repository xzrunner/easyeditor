#ifndef _EASYEDITOR_TEXTURE_PACKER_H_
#define _EASYEDITOR_TEXTURE_PACKER_H_

#include <SM_Vector.h>

#include <json/json.h>

#include <string>

namespace ee
{

class TexturePacker
{
public:
	TexturePacker(const std::string& src_data_dir);
	~TexturePacker();
	
	void Add(const std::string& filepath);

	struct Frame;
	const Frame* Query(const std::string& filepath) const;

	int GetTextureHeight(int idx) const;

	int QueryIdx(const std::string& filepath) const;

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
		sm::vec2 tex_coords[4];

		sm::vec2 offset;

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
		const std::string filepath;

		int idx;

		int width, height;

		std::string format;

		std::map<std::string, Frame> frames;

		bool is_easydb;

		Texture(const std::string& filepath) : filepath(filepath) {}

		void Load(const Json::Value& value, const std::string& src_data_dir);

		const Frame* Query(const std::string& filepath) const;
	};

public:
	const std::vector<const Texture*>& GetTextures() const { return m_textures; }

private:
	std::string m_src_data_dir;

	std::vector<const Texture*> m_textures;

}; // TexturePacker

}

#endif // _EASYEDITOR_TEXTURE_PACKER_H_
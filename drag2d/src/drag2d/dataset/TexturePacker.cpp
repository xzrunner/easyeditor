#include "TexturePacker.h"

#include "common/tools.h"
#include "common/FileNameTools.h"

#include <fstream>

namespace d2d
{

TexturePacker::TexturePacker(const std::string& src_data_dir)
	: m_src_data_dir(src_data_dir)
{
}

TexturePacker::~TexturePacker()
{
	for_each(m_textures.begin(), m_textures.end(), DeletePointerFunctor<Texture>());
}

void TexturePacker::Add(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	Texture* tex = new Texture;
	tex->idx = m_textures.size();
	tex->Load(value, m_src_data_dir);
	m_textures.push_back(tex);
}

const TexturePacker::Frame* TexturePacker::Query(const std::string& filepath) const
{
	for (int i = 0, n = m_textures.size(); i < n; ++i) {
		if (const TexturePacker::Frame* frame = m_textures[i]->Query(filepath)) {
			return frame;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// struct TexturePacker::Region
//////////////////////////////////////////////////////////////////////////

void TexturePacker::Region::
Load(const Json::Value& value)
{
	x = value["x"].asInt();
	y = value["y"].asInt();
	w = value["w"].asInt();
	h = value["h"].asInt();
}

//////////////////////////////////////////////////////////////////////////
// struct TexturePacker::FrameSrcData
//////////////////////////////////////////////////////////////////////////

void TexturePacker::FrameSrcData::
Load(const Json::Value& value, const std::string& src_data_dir)
{
	std::string filepath = value["filename"].asString();
	filename = d2d::FilenameTools::FormatFilepathAbsolute(src_data_dir + "\\" + filepath);

	frame.Load(value["frame"]);
	rotated = value["rotated"].asBool();
	trimmed = value["trimmed"].asBool();
	sprite_source_size.Load(value["spriteSourceSize"]);
	src_width = value["sourceSize"]["w"].asInt();
	src_height = value["sourceSize"]["h"].asInt();	
}

//////////////////////////////////////////////////////////////////////////
// struct TexturePacker::FrameDstData
//////////////////////////////////////////////////////////////////////////

void TexturePacker::FrameDstData::
Load(const FrameSrcData& src, const Texture* tex)
{
	bool flip_y = tex->is_easydb;

	float left, right, up, down;
	if (src.rotated)
	{
		left = src.frame.x;
		right = src.frame.x + src.frame.h;
		if (flip_y) {
			up = tex->height - src.frame.y;
			down = tex->height - (src.frame.y + src.frame.w);
		} else {
			up = src.frame.y;
			down = src.frame.y + src.frame.w;
		}

		tex_coords[0].set(left, up);
		tex_coords[1].set(right, up);
		tex_coords[2].set(right, down);
		tex_coords[3].set(left, down);
	}
	else
	{
		left = src.frame.x;
		right = src.frame.x + src.frame.w;
		if (flip_y) {
			up = tex->height - src.frame.y;
			down = tex->height - (src.frame.y + src.frame.h);
		} else {
			up = src.frame.y;
			down = src.frame.y + src.frame.h;
		}

		tex_coords[0].set(left, down);
		tex_coords[1].set(left, up);
		tex_coords[2].set(right, up);
		tex_coords[3].set(right, down);
	}

	offset.x = (src.sprite_source_size.x + src.sprite_source_size.w * 0.5f) - src.src_width * 0.5f;
	offset.y = src.src_height * 0.5f - (src.sprite_source_size.y + src.sprite_source_size.h * 0.5f);

}

//////////////////////////////////////////////////////////////////////////
// struct TexturePacker::Frame
//////////////////////////////////////////////////////////////////////////

void TexturePacker::Frame::
Load(const Json::Value& value, const std::string& src_data_dir, const Texture* tex)
{
	tex_idx = tex->idx;
	src.Load(value, src_data_dir);
	dst.Load(src, tex);
}

//////////////////////////////////////////////////////////////////////////
// struct TexturePacker::Texture
//////////////////////////////////////////////////////////////////////////

void TexturePacker::Texture::
Load(const Json::Value& value, const std::string& src_data_dir)
{
	width = value["meta"]["size"]["w"].asInt();
	height = value["meta"]["size"]["h"].asInt();

// 	std::string scale = value["meta"]["scale"].asString();
// 	m_invscale = atof(scale.c_str());

	std::string app = value["meta"]["app"].asString();
	is_easydb = app.compare("easydb") == 0;

	int i = 0;
	Json::Value frame_val = value["frames"][i++];
	while (!frame_val.isNull()) {
		Frame frame;
		frame.Load(frame_val, src_data_dir, this);
		frames.insert(std::make_pair(frame.src.filename, frame));
		frame_val = value["frames"][i++];
	}
}

const TexturePacker::Frame* TexturePacker::Texture::
Query(const std::string& filepath) const
{
	std::map<std::string, Frame>::const_iterator itr 
		= frames.find(filepath);
	if (itr != frames.end()) {
		return &itr->second;
	} else {
		return NULL;
	}
}

}
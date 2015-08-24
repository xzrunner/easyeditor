#ifndef _LIBCOCO_TEXTURE_PACKER_ADAPTER_H_
#define _LIBCOCO_TEXTURE_PACKER_ADAPTER_H_

#include <json/json.h>

namespace libcoco
{
namespace epd
{

class TPAdapter
{
public:
	struct Region
	{
		int x, y;
		int w, h;
	};

	struct Entry
	{
		std::string filename;

		Region frame;

		bool rotated;

		bool trimmed;
		Region sprite_source_size;

		int src_width, src_height;
	};

public:
	TPAdapter(const std::string& src_data_dir) 
		: m_src_data_dir(src_data_dir) {}

	void Load(const char* filename);

	const std::vector<Entry>& GetFrames() const { return m_frames; }

	float GetInvScale() const { return m_invscale; }

	int GetHeight() const { return m_height; }

	bool IsEasyDBData() const { return m_is_easydb; }

private:
	void Load(const Json::Value& value, Entry& entry);
	void Load(const Json::Value& value, Region& region);

private:
	std::string m_src_data_dir;

	int m_width, m_height;

	float m_invscale;

	std::vector<Entry> m_frames;

	bool m_is_easydb;

}; // TPAdapter

}
}

#endif // _LIBCOCO_TEXTURE_PACKER_ADAPTER_H_
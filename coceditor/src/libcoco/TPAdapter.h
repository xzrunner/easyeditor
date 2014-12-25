#ifndef _LIBCOCO_TEXTURE_PACKER_ADAPTER_H_
#define _LIBCOCO_TEXTURE_PACKER_ADAPTER_H_

#include <json/json.h>

namespace libcoco
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

	void Load(const char* filename);

	const std::vector<Entry>& GetFrames() const { return m_frames; }

	float GetInvScale() const { return m_invscale; }

private:
	void Load(const Json::Value& value, Entry& entry);
	void Load(const Json::Value& value, Region& region);

private:
	int m_width, m_height;

	float m_invscale;

	std::vector<Entry> m_frames;

}; // TPAdapter

}

#endif // _LIBCOCO_TEXTURE_PACKER_ADAPTER_H_
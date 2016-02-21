#ifndef _ECOCO_EPD_TEXTURE_MANAGER_H_
#define _ECOCO_EPD_TEXTURE_MANAGER_H_



#include "TPAdapter.h"

namespace ecoco
{
namespace epd
{

class TextureMgr
{
public:
	struct Entry
	{
		Entry(const std::string& src_data_dir) : adapter(src_data_dir) {}

		std::string filepath;
		TPAdapter adapter;
	};

public:
	TextureMgr();
	~TextureMgr();

	bool Add(const std::string& filepath, int index);

	const Entry* GetTexture(int idx) const;

	void SetSrcDataDir(const std::string& src_data_dir) {
		m_src_data_dir = src_data_dir;
	}

private:
	static const int MAX_NUM = 100;

private:
	std::string m_src_data_dir;

	Entry* m_textures[MAX_NUM];

}; // TexturesMgr

}
}

#endif // _ECOCO_EPD_TEXTURE_MANAGER_H_
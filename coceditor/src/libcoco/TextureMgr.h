#ifndef _LIBCOCO_TEXTURE_MANAGER_H_
#define _LIBCOCO_TEXTURE_MANAGER_H_

#include <drag2d.h>

#include "TPAdapter.h"

namespace libcoco
{

class TextureMgr
{
public:
	struct Entry
	{
		std::string filepath;
		TPAdapter adapter;
	};

public:
	TextureMgr();
	~TextureMgr();

	bool Add(const std::string& filepath, int index);

	const Entry* GetTexture(int idx) const;

private:
	static const int MAX_NUM = 100;

private:
	Entry* m_textures[MAX_NUM];

}; // TexturesMgr

}

#endif // _LIBCOCO_TEXTURE_MANAGER_H_
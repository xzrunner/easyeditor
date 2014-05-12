// 更改TexturePacker合图的json文件

#ifndef _EASYDB_FIX_TEXTURE_PACKER_JSON_H_
#define _EASYDB_FIX_TEXTURE_PACKER_JSON_H_

#include <drag2d.h>

namespace edb
{

class FixTexPackerJson
{
public:
	FixTexPackerJson(const std::string& dir);

	void TranslateFrameXY(int dx, int dy);

private:
	wxArrayString m_files;

}; // FixTexPackerJson

}

#endif // _EASYDB_FIX_TEXTURE_PACKER_JSON_H_
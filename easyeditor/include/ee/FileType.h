#ifndef _EASYEDITOR_FILE_TYPE_H_
#define _EASYEDITOR_FILE_TYPE_H_

#include <string>

namespace ee
{

enum FileFormat
{
	FILE_INVALID = 0,
	// shape
	FILE_SHAPE,
	// symbol
	FILE_IMAGE,
	FILE_MESH,
	FILE_COMPLEX,
	FILE_ANIM,
	FILE_ANIS,
	FILE_SCALE9,
	FILE_FONTBLANK,
	FILE_FREETYPE,
	FILE_SCRIPTS,
	FILE_PARTICLE2D,
	FILE_PARTICLE3D,
	FILE_P3DINV,
	FILE_EJOY2D,
	FILE_TEXTURE,
	FILE_TERRAIN2D,
	FILE_ICON,
	FILE_SHADOW,
	FILE_UI,
	FILE_TEXT,
	FILE_MASK,
	FILE_PSD,
	FILE_UIWND,
	FILE_TRAIL,
	FILE_BONE,
};

class FileType
{
public:
	static FileFormat GetType(const std::string& filename);
	static std::string GetTag(FileFormat format);

	static bool IsType(const std::string& filename, FileFormat format);

}; // FileType

}

#endif // _EASYEDITOR_FILE_TYPE_H_
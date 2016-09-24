#include "FileType.h"
#include "StringHelper.h"

namespace ee
{

static const std::string TAG_SHAPE		= "shape";
static const std::string TAG_MESH		= "mesh";
static const std::string TAG_COMPLEX	= "complex";
static const std::string TAG_ANIM		= "anim";
static const std::string TAG_ANIS		= "anis";
static const std::string TAG_PATCH		= "scale9";
static const std::string TAG_FONTBLANK	= "fontblank";
static const std::string TAG_SCRIPTS	= "scripts";
static const std::string TAG_PARTICLE2D	= "particle2d";
static const std::string TAG_PARTICLE3D	= "particle";
static const std::string TAG_P3DINV		= "p3dinv";
static const std::string TAG_EJOY2D		= "ejoy2d";
static const std::string TAG_TEXTURE	= "texture";
static const std::string TAG_TERRAIN2D	= "terrain2d";
static const std::string TAG_ICON		= "icon";
static const std::string TAG_SHADOW		= "shadow";
static const std::string TAG_UI			= "ui";
static const std::string TAG_TEXT		= "text";
static const std::string TAG_MASK		= "mask";
static const std::string TAG_PSD		= "psd";
static const std::string TAG_UIWND		= "uiwnd";
static const std::string TAG_TRAIL		= "trail";
static const std::string TAG_SKELETON	= "skeleton";

static const std::string TAG_GEN		= "gen";

FileFormat FileType::GetType(const std::string& filename)
{
	if (filename.empty()) return FILE_INVALID;

	int pos = filename.rfind('.');
	if (pos == -1) return FILE_INVALID;

	std::string extension = filename.substr(pos);
	if (extension == ".json")
	{
		const std::string jsonName = filename.substr(0, filename.find_last_of('.'));
		if (jsonName.find('_') == -1) return FILE_INVALID;

		std::string ext = jsonName.substr(jsonName.find_last_of('_') + 1);
		if (ext.find(TAG_GEN) != std::string::npos) {
			ext = ext.substr(0, ext.find(TAG_GEN) - 1);
		}

		if (ext == TAG_SHAPE) return FILE_SHAPE;
		else if (ext == TAG_COMPLEX) return FILE_COMPLEX;
		else if (ext == TAG_ANIM) return FILE_ANIM;
		else if (ext == TAG_ANIS) return FILE_ANIS;
		else if (ext == TAG_PATCH) return FILE_SCALE9;
		else if (ext == TAG_FONTBLANK) return FILE_FONTBLANK;
		else if (ext == TAG_MESH) return FILE_MESH;
		else if (ext == TAG_PARTICLE3D) return FILE_PARTICLE3D;
		else if (ext == TAG_P3DINV) return FILE_P3DINV;
		else if (ext == TAG_PARTICLE2D) return FILE_PARTICLE2D;
		else if (ext == TAG_EJOY2D) return FILE_EJOY2D;
		else if (ext == TAG_TEXTURE) return FILE_TEXTURE;
		else if (ext == TAG_TERRAIN2D) return FILE_TERRAIN2D;
		else if (ext == TAG_ICON) return FILE_ICON;
		else if (ext == TAG_SHADOW) return FILE_SHADOW;
		else if (ext == TAG_UI) return FILE_UI;
		else if (ext == TAG_TEXT) return FILE_TEXT;
		else if (ext == TAG_MASK) return FILE_MASK;
		else if (ext == TAG_PSD) return FILE_PSD;
		else if (ext == TAG_UIWND) return FILE_UIWND;
		else if (ext == TAG_TRAIL) return FILE_TRAIL;
		else if (ext == TAG_SKELETON) return FILE_SKELETON;
		else return FILE_INVALID;
	}
	else if (extension == ".lua")
	{
		const std::string luaName = filename.substr(0, filename.find_last_of('.'));
		if (luaName.find('_') == -1) return FILE_INVALID;

		const std::string jsonExtension = luaName.substr(luaName.find_last_of('_') + 1);
		if (jsonExtension == TAG_SCRIPTS) return FILE_SCRIPTS;
		else return FILE_INVALID;
	}
	else if (extension == ".ttf")
	{
		return FILE_FREETYPE;
	}
	else
	{
		StringHelper::ToLower(extension);
		if (extension == ".jpg" || extension == ".png" || extension == ".bmp") return FILE_IMAGE;
		else return FILE_INVALID;
	}
}

std::string FileType::GetTag(FileFormat format)
{
	std::string ext;
	switch (format)
	{
	case FILE_SHAPE:
		ext = TAG_SHAPE;
		break;
	case FILE_MESH:
		ext = TAG_MESH;
		break;
	case FILE_COMPLEX:
		ext = TAG_COMPLEX;
		break;
	case FILE_ANIM:
		ext = TAG_ANIM;
		break;
	case FILE_ANIS:
		ext = TAG_ANIS;
		break;
	case FILE_SCALE9:
		ext = TAG_PATCH;
		break;
	case FILE_FONTBLANK:
		ext = TAG_FONTBLANK;
		break;
	case FILE_SCRIPTS:
		ext = TAG_SCRIPTS;
		break;
	case FILE_PARTICLE2D:
		ext = TAG_PARTICLE2D;
		break;
	case FILE_PARTICLE3D:
		ext = TAG_PARTICLE3D;
		break;
	case FILE_P3DINV:
		ext = TAG_P3DINV;
		break;
	case FILE_EJOY2D:
		ext = TAG_EJOY2D;
		break;
	case FILE_TEXTURE:
		ext = TAG_TEXTURE;
		break;
	case FILE_TERRAIN2D:
		ext = TAG_TERRAIN2D;
		break;
	case FILE_ICON:
		ext = TAG_ICON;
		break;
	case FILE_SHADOW:
		ext = TAG_SHADOW;
		break;
	case FILE_UI:
		ext = TAG_UI;
		break;
	case FILE_TEXT:
		ext = TAG_TEXT;
		break;
	case FILE_MASK:
		ext = TAG_MASK;
		break;
	case FILE_PSD:
		ext = TAG_PSD;
		break;
	case FILE_UIWND:
		ext = TAG_UIWND;
		break;
	case FILE_TRAIL:
		ext = TAG_TRAIL;
		break;
	case FILE_SKELETON:
		ext = TAG_SKELETON;
		break;
	}
	return ext;
}

bool FileType::IsType(const std::string& filename, FileFormat format)
{
	FileFormat file_type = GetType(filename);
	return file_type == format;
}

}
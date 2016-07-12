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

static const std::string TAG_GEN		= "gen";

FileType::Type FileType::GetType(const std::string& filename)
{
	if (filename.empty()) return e_unknown;

	int pos = filename.rfind('.');
	if (pos == -1) return e_unknown;

	std::string extension = filename.substr(pos);
	if (extension == ".json")
	{
		const std::string jsonName = filename.substr(0, filename.find_last_of('.'));
		if (jsonName.find('_') == -1) return e_unknown;

		std::string ext = jsonName.substr(jsonName.find_last_of('_') + 1);
		if (ext.find(TAG_GEN) != std::string::npos) {
			ext = ext.substr(0, ext.find(TAG_GEN) - 1);
		}

		if (ext == TAG_SHAPE) return e_shape;
		else if (ext == TAG_COMPLEX) return e_complex;
		else if (ext == TAG_ANIM) return e_anim;
		else if (ext == TAG_ANIS) return e_anis;
		else if (ext == TAG_PATCH) return e_scale9;
		else if (ext == TAG_FONTBLANK) return e_fontblank;
		else if (ext == TAG_MESH) return e_mesh;
		else if (ext == TAG_PARTICLE3D) return e_particle3d;
		else if (ext == TAG_P3DINV) return e_p3dinv;
		else if (ext == TAG_PARTICLE2D) return e_particle2d;
		else if (ext == TAG_EJOY2D) return e_ejoy2d;
		else if (ext == TAG_TEXTURE) return e_texture;
		else if (ext == TAG_TERRAIN2D) return e_terrain2d;
		else if (ext == TAG_ICON) return e_icon;
		else if (ext == TAG_SHADOW) return e_shadow;
		else if (ext == TAG_UI) return e_ui;
		else if (ext == TAG_TEXT) return e_text;
		else if (ext == TAG_MASK) return e_mask;
		else if (ext == TAG_PSD) return e_psd;
		else if (ext == TAG_UIWND) return e_uiwnd;
		else return e_unknown;
	}
	else if (extension == ".lua")
	{
		const std::string luaName = filename.substr(0, filename.find_last_of('.'));
		if (luaName.find('_') == -1) return e_unknown;

		const std::string jsonExtension = luaName.substr(luaName.find_last_of('_') + 1);
		if (jsonExtension == TAG_SCRIPTS) return e_scripts;
		else return e_unknown;
	}
	else if (extension == ".ttf")
	{
		return e_freetype;
	}
	else
	{
		StringHelper::ToLower(extension);
		if (extension == ".jpg" || extension == ".png" || extension == ".bmp") return e_image;
		else return e_unknown;
	}
}

std::string FileType::GetTag(Type type)
{
	std::string ext;
	switch (type)
	{
	case e_shape:
		ext = TAG_SHAPE;
		break;
	case e_mesh:
		ext = TAG_MESH;
		break;
	case e_complex:
		ext = TAG_COMPLEX;
		break;
	case e_anim:
		ext = TAG_ANIM;
		break;
	case e_anis:
		ext = TAG_ANIS;
		break;
	case e_scale9:
		ext = TAG_PATCH;
		break;
	case e_fontblank:
		ext = TAG_FONTBLANK;
		break;
	case e_scripts:
		ext = TAG_SCRIPTS;
		break;
	case e_particle2d:
		ext = TAG_PARTICLE2D;
		break;
	case e_particle3d:
		ext = TAG_PARTICLE3D;
		break;
	case e_p3dinv:
		ext = TAG_P3DINV;
		break;
	case e_ejoy2d:
		ext = TAG_EJOY2D;
		break;
	case e_texture:
		ext = TAG_TEXTURE;
		break;
	case e_terrain2d:
		ext = TAG_TERRAIN2D;
		break;
	case e_icon:
		ext = TAG_ICON;
		break;
	case e_shadow:
		ext = TAG_SHADOW;
		break;
	case e_ui:
		ext = TAG_UI;
		break;
	case e_text:
		ext = TAG_TEXT;
		break;
	case e_mask:
		ext = TAG_MASK;
		break;
	case e_psd:
		ext = TAG_PSD;
		break;
	case e_uiwnd:
		ext = TAG_UIWND;
		break;
	}
	return ext;
}

bool FileType::IsType(const std::string& filename, Type type)
{
	FileType::Type file_type = GetType(filename);
	return file_type == type;
}

}
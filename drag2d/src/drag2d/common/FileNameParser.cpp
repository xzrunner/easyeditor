#include "FileNameParser.h"

#include "common/tools.h"

namespace d2d
{

static const wxString TAG_SHAPE = "shape";
static const wxString TAG_MESH = "mesh";
static const wxString TAG_COMPLEX = "complex";
static const wxString TAG_ANIM = "anim";
static const wxString TAG_PATCH = "scale9";
static const wxString TAG_FONTBLANK = "fontblank";
static const wxString TAG_SCRIPTS = "scripts";
static const wxString TAG_PARTICLE2D = "particle2d";
static const wxString TAG_PARTICLE3D = "particle";
static const wxString TAG_EJOY2D = "ejoy2d";

FileNameParser::Type FileNameParser::getFileType(const wxString& filename)
{
	if (filename.empty()) return e_unknown;

	int pos = filename.rfind('.');
	if (pos == -1) return e_unknown;

	std::string extension = filename.substr(pos);
	if (extension == ".json")
	{
		const wxString jsonName = filename.substr(0, filename.find_last_of('.'));
		if (jsonName.find('_') == -1) return e_unknown;

		const wxString jsonExtension = jsonName.substr(jsonName.find_last_of('_') + 1);

		if (jsonExtension == TAG_SHAPE) return e_shape;
		else if (jsonExtension == TAG_COMPLEX) return e_complex;
		else if (jsonExtension == TAG_ANIM) return e_anim;
		else if (jsonExtension == TAG_PATCH) return e_scale9;
		else if (jsonExtension == TAG_FONTBLANK) return e_fontblank;
		else if (jsonExtension == TAG_MESH) return e_mesh;
		else if (jsonExtension == TAG_PARTICLE3D) return e_particle3d;
		else if (jsonExtension == TAG_PARTICLE2D) return e_particle2d;
		else if (jsonExtension == TAG_EJOY2D) return e_ejoy2d;
		else return e_unknown;
	}
	else if (extension == ".lua")
	{
		const wxString luaName = filename.substr(0, filename.find_last_of('.'));
		if (luaName.find('_') == -1) return e_unknown;

		const wxString jsonExtension = luaName.substr(luaName.find_last_of('_') + 1);
		if (jsonExtension == TAG_SCRIPTS) return e_scripts;
		else return e_unknown;
	}
	else if (extension == ".ttf")
	{
		return e_freetype;
	}
	else
	{
		StringTools::toLower(extension);
		if (extension == ".jpg" || extension == ".png" || extension == ".bmp") return e_image;
		else return e_unknown;
	}
}

wxString FileNameParser::getFileTag(Type type)
{
	wxString extension;
	switch (type)
	{
	case e_shape:
		extension = TAG_SHAPE;
		break;
	case e_mesh:
		extension = TAG_MESH;
		break;
	case e_complex:
		extension = TAG_COMPLEX;
		break;
	case e_anim:
		extension = TAG_ANIM;
		break;
	case e_scale9:
		extension = TAG_PATCH;
		break;
	case e_fontblank:
		extension = TAG_FONTBLANK;
		break;
	case e_scripts:
		extension = TAG_SCRIPTS;
		break;
	case e_particle2d:
		extension = TAG_PARTICLE2D;
		break;
	case e_particle3d:
		extension = TAG_PARTICLE3D;
		break;
	case e_ejoy2d:
		extension = TAG_EJOY2D;
		break;
	}
	return extension;
}

bool FileNameParser::isType(const wxString& filename, Type type)
{
	FileNameParser::Type fileType = getFileType(filename);
	return fileType == type;
}

} // d2d
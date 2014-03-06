#include "FileNameParser.h"

#include "common/tools.h"

namespace d2d
{

static const wxString TAG_POLYLINE = "polyline";
static const wxString TAG_CIRCLE = "circle";
static const wxString TAG_POLYGON = "surface";
static const wxString TAG_SHAPE = "shape";

static const wxString TAG_MESH = "mesh";
static const wxString TAG_COMBINATION = "combination";
static const wxString TAG_COMPLEX = "complex";
static const wxString TAG_ANIM = "anim";
static const wxString TAG_PATCH = "scale9";
static const wxString TAG_FONTBLANK = "fontblank";
static const wxString TAG_SCRIPTS = "scripts";

FileNameParser::Type FileNameParser::getFileType(const wxString& filename)
{
	if (filename.empty()) return e_unknown;

	int pos = filename.rfind('.');
	if (pos == -1) return e_unknown;

	std::string extension = filename.substr(pos);
	if (extension == ".txt")
	{
		const wxString txtName = filename.substr(0, filename.find_last_of('.'));
		if (txtName.find('_') == -1) return e_unknown;

		const wxString txtExtension = txtName.substr(txtName.find_last_of('_') + 1);

		if (txtExtension == TAG_POLYLINE) return e_polyline;
		else if (txtExtension == TAG_CIRCLE) return e_circle;
		else if (txtExtension == TAG_POLYGON) return e_polygon;
		else if (txtExtension == TAG_MESH) return e_mesh;
		else if (txtExtension == TAG_COMBINATION) return e_combination;
		else return e_unknown;
	}
	else if (extension == ".json")
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
	case e_polyline:
		extension = TAG_POLYLINE;
		break;
	case e_circle:
		extension = TAG_CIRCLE;
		break;
	case e_polygon:
		extension = TAG_POLYGON;
		break;
	case e_shape:
		extension = TAG_SHAPE;
		break;
	case e_mesh:
		extension = TAG_MESH;
		break;
	case e_combination:
		extension = TAG_COMBINATION;
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
	}
	return extension;
}

bool FileNameParser::isType(const wxString& filename, Type type)
{
	FileNameParser::Type fileType = getFileType(filename);
	return fileType == type;
}

} // d2d
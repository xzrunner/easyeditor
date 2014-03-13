#include "CocoPacker.h"

#include <drag2d.h>
#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace eparticle3d
{

void CocoPacker::pack(const wxString& srcdir, const wxString& dstfilename)
{
	ebuilder::CodeGenerator gen;

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(srcdir.ToStdString(), files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_particle3d))
		{
			pack(filepath, gen);
		}
	}

	std::ofstream fout(dstfilename.fn_str());
	if (!fout.fail())
	{
		fout << gen.toText();
		fout.close();
	}
}

void CocoPacker::pack(const wxString& filepath, ebuilder::CodeGenerator& gen)
{
	Json::Value val;
	Json::Reader reader;
	std::ifstream fin(filepath.fn_str());
	reader.parse(fin, val);
	fin.close();

	std::string s = val["name"].asString();
	lua::TableAssign ta(gen, "['"+s+"']", true);

	lua::assign(gen, "['name']", "'"+s+"',");

	s = val["package"].asString();
	s = "misc";
	lua::assign(gen, "['package']", "'"+s+"',");

	s = wxString::FromDouble(val["count"].asInt());
	lua::assign(gen, "['count']", s+",");

	s = wxString::FromDouble(val["layer"].asInt());
	lua::assign(gen, "['layer']", s+",");

	s = wxString::FromDouble(val["emission_time"].asInt());
	lua::assign(gen, "['emission_time']", s+"*l,");

	s = wxString::FromDouble(val["min_life"].asInt());
	lua::assign(gen, "['min_life']", s+"*l,");

	s = wxString::FromDouble(val["max_life"].asInt());
	lua::assign(gen, "['max_life']", s+"*l,");

	s = wxString::FromDouble(val["min_hori"].asInt());
	lua::assign(gen, "['min_hori']", s+",");

	s = wxString::FromDouble(val["max_hori"].asInt());
	lua::assign(gen, "['max_hori']", s+",");

	s = wxString::FromDouble(val["min_vert"].asInt());
	lua::assign(gen, "['min_vert']", s+",");

	s = wxString::FromDouble(val["max_vert"].asInt());
	lua::assign(gen, "['max_vert']", s+",");

	s = wxString::FromDouble(val["min_spd"].asInt());
	lua::assign(gen, "['min_spd']", s+"*k,");

	s = wxString::FromDouble(val["max_spd"].asInt());
	lua::assign(gen, "['max_spd']", s+"*k,");

	s = wxString::FromDouble(val["gravity"].asInt());
	lua::assign(gen, "['gravity']", s+"*g,");

	s = wxString::FromDouble(val["inertia"].asInt());
	lua::assign(gen, "['inertia']", s+",");

	s = wxString::FromDouble(val["fadeout_time"].asInt());
	lua::assign(gen, "['fadeout_time']", s+"*l,");

	s = val["bounce"].asBool() ? "true" : "false";
	lua::assign(gen, "['bounce']", s+",");

	s = val["additive_blend"].asBool() ? "true" : "false";
	lua::assign(gen, "['additive_blend']", s+",");

	s = wxString::FromDouble(val["start_radius"].asInt());
	lua::assign(gen, "['start_radius']", s+",");

	s = val["orient_to_movement"].asBool() ? "true" : "false";
	lua::assign(gen, "['orient_to_movement']", s+",");

	s = val["orient_to_parent"].asBool() ? "true" : "false";
	lua::assign(gen, "['orient_to_parent']", s+",");

	wxString dir = d2d::FilenameTools::getFileDir(filepath);
	{
		lua::TableAssign ta(gen, "['components']", true);

		int i = 0;
		Json::Value child_val = val["components"][i++];
		while (!child_val.isNull()) {
			lua::TableAssign ta(gen, "", true);

			{
				wxString filepath = d2d::FilenameTools::getAbsolutePath(dir, child_val["filepath"].asString());

				Json::Value value;
				Json::Reader reader;
				std::ifstream fin(filepath.fn_str());
				reader.parse(fin, value);
				fin.close();

				s = value["name"].asString();
			}
//			s = child_val["name"].asString();
			lua::assign(gen, "['name']", "'"+s+"',");

			s = wxString::FromDouble(val["start_scale"].asInt());
			lua::assign(gen, "['start_scale']", s+"*s,");

			s = wxString::FromDouble(val["end_scale"].asInt());
			lua::assign(gen, "['end_scale']", s+"*s,");

			s = wxString::FromDouble(val["min_rotate"].asInt());
			lua::assign(gen, "['min_rotate']", s+"*r,");

			s = wxString::FromDouble(val["max_rotate"].asInt());
			lua::assign(gen, "['max_rotate']", s+"*r,");

			s = wxString::FromDouble(val["start_z"].asInt());
			lua::assign(gen, "['start_z']", s+",");

			child_val = val["components"][i++];
		}
	}	
}

}
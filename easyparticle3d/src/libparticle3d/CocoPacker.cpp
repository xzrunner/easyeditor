#include "CocoPacker.h"

#include <drag2d.h>
#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace eparticle3d
{

void CocoPacker::PackDir(const wxString& srcdir, const wxString& dstfilename)
{
	ebuilder::CodeGenerator gen;

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(srcdir.ToStdString(), files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_particle3d)) {
			PackSingle(filepath, gen);
		}
	}

	std::locale::global(std::locale(""));
	std::ofstream fout(dstfilename.fn_str());
	std::locale::global(std::locale("C"));
	if (!fout.fail())
	{
		fout << gen.toText();
		fout.close();
	}
}

void CocoPacker::PackSingle(const wxString& filepath, ebuilder::CodeGenerator& gen)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	int version = val["version"].asInt();
	if (version == 0) {
		PackOldVersion(val, gen, dir);
	} else {
		PackNewVersion(val, gen, dir);
	}
}

void CocoPacker::PackOldVersion(const Json::Value& val, ebuilder::CodeGenerator& gen,
								const std::string& dir)
{
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

	const float SPEED_SCALE = 0.25f;

	s = wxString::FromDouble(val["min_spd"].asInt() * SPEED_SCALE);
	lua::assign(gen, "['min_spd']", s+"*k,");

	s = wxString::FromDouble(val["max_spd"].asInt() * SPEED_SCALE);
	lua::assign(gen, "['max_spd']", s+"*k,");

	s = wxString::FromDouble(val["gravity"].asInt());
	lua::assign(gen, "['gravity']", s+"*g,");

	//	s = wxString::FromDouble(val["inertia"].asInt());
	s = wxString::FromDouble(10);
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
				std::locale::global(std::locale(""));
				std::ifstream fin(filepath.fn_str());
				std::locale::global(std::locale("C"));
				reader.parse(fin, value);
				fin.close();

				s = value["name"].asString();
			}
			//			s = child_val["name"].asString();
			lua::assign(gen, "['name']", "'"+s+"',");

			s = wxString::FromDouble(child_val["start_scale"].asInt());
			lua::assign(gen, "['start_scale']", s+"*s,");

			s = wxString::FromDouble(child_val["end_scale"].asInt());
			lua::assign(gen, "['end_scale']", s+"*s,");

			s = wxString::FromDouble(child_val["min_rotate"].asInt());
			lua::assign(gen, "['min_rotate']", s+"*r,");

			s = wxString::FromDouble(child_val["max_rotate"].asInt());
			lua::assign(gen, "['max_rotate']", s+"*r,");

			s = wxString::FromDouble(child_val["start_z"].asInt());
			lua::assign(gen, "['start_z']", s+",");

			child_val = val["components"][i++];
		}
	}	
}

void CocoPacker::PackNewVersion(const Json::Value& val, ebuilder::CodeGenerator& gen,
								const std::string& dir)
{
	std::string s = val["name"].asString();
	lua::TableAssign ta(gen, "['"+s+"']", true);

	lua::assign(gen, "['name']", "'"+s+"',");

	s = val["package"].asString();
	s = "misc";
	lua::assign(gen, "['package']", "'"+s+"',");

	s = wxString::FromDouble(val["count"].asDouble());
	lua::assign(gen, "['count']", s+",");
	s = wxString::FromDouble(val["layer"].asInt());
	lua::assign(gen, "['layer']", s+",");

	s = wxString::FromDouble(val["emission_time"].asDouble());
	lua::assign(gen, "['emission_time']", s+"*l,");

	float center = val["life"]["center"].asDouble(),
		offset = val["life"]["offset"].asDouble();

	s = wxString::FromDouble(center - offset);
	lua::assign(gen, "['min_life']", s+"*l,");

	s = wxString::FromDouble(center + offset);
	lua::assign(gen, "['max_life']", s+"*l,");

	s = wxString::FromDouble(val["min_hori"].asInt());
	lua::assign(gen, "['min_hori']", s+",");

	s = wxString::FromDouble(val["max_hori"].asInt());
	lua::assign(gen, "['max_hori']", s+",");

	s = wxString::FromDouble(val["min_vert"].asInt());
	lua::assign(gen, "['min_vert']", s+",");

	s = wxString::FromDouble(val["max_vert"].asInt());
	lua::assign(gen, "['max_vert']", s+",");

	const float SPEED_SCALE = 0.25f;

	center = val["speed"]["center"].asDouble();
	offset = val["speed"]["offset"].asDouble();

	s = wxString::FromDouble((center - offset) * SPEED_SCALE);
	lua::assign(gen, "['min_spd']", s+"*k,");

	s = wxString::FromDouble((center + offset) * SPEED_SCALE);
	lua::assign(gen, "['max_spd']", s+"*k,");

	s = wxString::FromDouble(val["gravity"].asDouble());
	lua::assign(gen, "['gravity']", s+"*g,");

	//	s = wxString::FromDouble(val["inertia"].asInt());
	s = wxString::FromDouble(10);
	lua::assign(gen, "['inertia']", s+",");

	s = wxString::FromDouble(val["fadeout_time"].asDouble());
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
				std::locale::global(std::locale(""));
				std::ifstream fin(filepath.fn_str());
				std::locale::global(std::locale("C"));
				reader.parse(fin, value);
				fin.close();

				s = value["name"].asString();
			}
			//			s = child_val["name"].asString();
			lua::assign(gen, "['name']", "'"+s+"',");

			s = wxString::FromDouble(child_val["scale"]["start"].asDouble());
			lua::assign(gen, "['start_scale']", s+"*s,");

			s = wxString::FromDouble(child_val["scale"]["end"].asDouble());
			lua::assign(gen, "['end_scale']", s+"*s,");

			center = child_val["rotate"]["center"].asDouble();
			offset = child_val["rotate"]["offset"].asDouble();

			s = wxString::FromDouble(center - offset);
			lua::assign(gen, "['min_rotate']", s+"*r,");

			s = wxString::FromDouble(center + offset);
			lua::assign(gen, "['max_rotate']", s+"*r,");

			s = wxString::FromDouble(child_val["start_z"].asInt());
			lua::assign(gen, "['start_z']", s+",");

			child_val = val["components"][i++];
		}
	}	
}

}
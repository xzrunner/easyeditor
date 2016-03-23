#include "TransOldP3DFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <wx/arrstr.h>

#include <fstream>

namespace edb
{

std::string TransOldP3DFile::Command() const
{
	return "trans-old-p3d";
}

std::string TransOldP3DFile::Description() const
{
	return "trans old p3d file";
}

std::string TransOldP3DFile::Usage() const
{
	// trans-old-p3d dir
	std::string usage = Command() + " [folder]";
	return usage;
}

int TransOldP3DFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Run(argv[2]);

	return 0;
}

void TransOldP3DFile::Run(const std::string& folder)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(folder, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_particle3d)) {
			Trans(filepath);
		}
	}
}

void TransOldP3DFile::Trans(const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);

	bool dirty = false;

	// start pos
	if (!value["start_radius_3d"].isNull()) {
		dirty = true;
		bool start_radius_3d = value["start_radius_3d"].asBool();
		float start_radius = value["start_radius"].asDouble();
		if (start_radius_3d) {
			float angle = (value["max_vert"].asDouble() + value["min_vert"].asDouble()) * 0.5f;
			value["start_pos"]["radius"] = fabs(start_radius * cos(angle));
			value["start_pos"]["height"] = start_radius * sin(angle);
		} else {
			value["start_pos"]["radius"] = fabs(start_radius);
			value["start_pos"]["height"] = 0;
		}
	}

	// emission count
	float time = value["emission_time"].asDouble();
	if (time < 1000 / 30.0f) {
		float f = 1000 / 30.0f / time;
		dirty = true;
		value["count"] = value["count"].asDouble() * f;
		value["emission_time"] = value["emission_time"].asDouble() * f;
	}
	
	if (dirty) {
		std::cout << "FIX P3D: " << filepath << std::endl;
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

}
#include "ChangeJsonName.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>

#include <fstream>
#include <set>

namespace edb
{

std::string ChangeJsonName::Command() const
{
	return "rename-json";
}

std::string ChangeJsonName::Description() const
{
	return "change json name";
}

std::string ChangeJsonName::Usage() const
{
	return Command() + " [dir path] [postfix]";
}

int ChangeJsonName::Run(int argc, char *argv[])
{
	// rename-json E:\test2\2000cunminnan particle

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void ChangeJsonName::Trigger(const std::string& dir, const std::string& postfix) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);

	std::set<std::string> used_set;
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (!ee::FileType::IsType(filepath, ee::FILE_PARTICLE3D)) {
			continue;
		}

		std::string name = ee::FileHelper::GetFilename(filepath);
		ee::StringHelper::ToLower(name);
		name = name.substr(0, name.find_last_of('_'));
		used_set.insert(name);
	}

	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (!ee::FileType::IsType(filepath, ee::FILE_COMPLEX)) {
			continue;
		}

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		if (value["name"].isNull()) {
			continue;
		}

		std::string name = ee::FileHelper::GetFilename(filepath);
		ee::StringHelper::ToLower(name);
		name = name.substr(0, name.find_last_of('_'));
		if (used_set.find(name) != used_set.end()) {
			value["name"] = name + postfix;
		} else {
			value["name"] = name;
		}

		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

}
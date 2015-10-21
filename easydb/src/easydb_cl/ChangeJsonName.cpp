#include "ChangeJsonName.h"
#include "check_params.h"

#include <wx/filename.h>
#include <fstream>
#include <drag2d.h>

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

void ChangeJsonName::Run(int argc, char *argv[])
{
	// rename-json E:\test2\2000cunminnan particle

	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2], argv[3]);
}

void ChangeJsonName::Trigger(const std::string& dir, const std::string& postfix) const
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);

	std::set<std::string> used_set;
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();

		if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_particle3d)) {
			continue;
		}

		std::string name = d2d::FilenameTools::getFilename(filepath);
		d2d::StringTools::ToLower(name);
		name = name.substr(0, name.find_last_of('_'));
		used_set.insert(name);
	}

	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();

		if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
			continue;
		}

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.fn_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		if (value["name"].isNull()) {
			continue;
		}

		std::string name = d2d::FilenameTools::getFilename(filepath);
		d2d::StringTools::ToLower(name);
		name = name.substr(0, name.find_last_of('_'));
		if (used_set.find(name) != used_set.end()) {
			value["name"] = name + postfix;
		} else {
			value["name"] = name;
		}

		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.fn_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

}
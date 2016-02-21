#include "SetNameFromFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <fstream>

namespace edb
{

SetNameFromFile::SetNameFromFile()
	: m_do_complex(false)
{
}

std::string SetNameFromFile::Command() const
{
	return "set-name";
}

std::string SetNameFromFile::Description() const
{
	return "set \"name\" from filename";
}

std::string SetNameFromFile::Usage() const
{
	// set-name E:\test2\1001

	return Command() + " [dir path] (-c)or(--complex)";
}

void SetNameFromFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	if (argc >= 4) {
		m_do_complex = check_params(argv[3], "-c", "--complex");
	}

	AddNameFromFile(argv[2]);
}

void SetNameFromFile::AddNameFromFile(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_anim)) {
			AddName(filepath);
		} else if (m_do_complex && 
			ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
			AddName(filepath);		
		}
	}
}

void SetNameFromFile::AddName(const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string oldname = value["name"].asString();

	size_t begin = filepath.find_last_of('\\') + 1;
	size_t end = filepath.find_last_of('_');
	std::string newname = filepath.substr(begin, end - begin);

	if (oldname.empty() || oldname != newname)
	{
		value["name"] = newname;

		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));
		writer.write(fout, value);
		fout.close();
	}
}

}
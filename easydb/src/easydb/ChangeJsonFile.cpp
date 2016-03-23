#include "ChangeJsonFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <fstream>

namespace edb
{

std::string ChangeJsonFile::Command() const
{
	return "change-json";
}

std::string ChangeJsonFile::Description() const
{
	return "change json files' value";
}

std::string ChangeJsonFile::Usage() const
{
	// change-json e:/test2/1001 -s angle 1.1

	std::string usage = Command() + " [dir path] [options] <parms>";
	usage += "-s, --scale \t scale the value: [key] [times]";
	return usage;
}

int ChangeJsonFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 6)) return -1;
	if (!check_folder(argv[2])) return -1;
	
	// todo
	std::string opt = argv[3];
	if (opt != "-s" && opt != "--scale") {
		std::cerr << "error opt" << std::endl;
		std::cerr << Usage() << std::endl;
		return -1;
	}

	Run(argv[2]);
	if (opt == "-s" || opt == "--scale") {
		Scale(argv[4], atof(argv[5]));
	} else {
		std::cout << "unknown option" << std::endl;
		return -1;
	}

	return 0;
}

void ChangeJsonFile::Run(const std::string& dir/*, ee::FileType::Type type*/)
//	: m_type(type)
{
	ee::FileHelper::FetchAllFiles(dir, m_files);
}

void ChangeJsonFile::Scale(const std::string& key, float times)
{
	for (int i = 0, n = m_files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(m_files[i].ToStdString());
//		if (ee::FileType::isType(filepath, m_type))
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.c_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			if (check_json_key(value, key, filepath))
			{
				float old = value[key].asDouble();
				value[key] = (int)old * times;

				Json::StyledStreamWriter writer;
				std::locale::global(std::locale(""));
				std::ofstream fout(filepath.c_str());
				std::locale::global(std::locale("C"));
				writer.write(fout, value);
				fout.close();
			}
		}
	}
}

}
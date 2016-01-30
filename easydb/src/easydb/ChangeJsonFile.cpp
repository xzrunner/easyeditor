#include "ChangeJsonFile.h"
#include "check_params.h"

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

void ChangeJsonFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 6)) return;
	if (!check_folder(argv[2])) return;
	
	// todo
	std::string opt = argv[3];
	if (opt != "-s" && opt != "--scale") {
		std::cerr << "error opt" << std::endl;
		std::cerr << Usage() << std::endl;
		return;
	}

	Run(argv[2]);
	if (opt == "-s" || opt == "--scale") {
		Scale(argv[4], atof(argv[5]));
	} else {
		std::cerr << "unknown option" << std::endl;
	}
}

void ChangeJsonFile::Run(const std::string& dir/*, d2d::FileType::Type type*/)
//	: m_type(type)
{
	d2d::FileHelper::FetchAllFiles(dir, m_files);
}

void ChangeJsonFile::Scale(const std::string& key, float times)
{
	for (int i = 0, n = m_files.size(); i < n; ++i)
	{
		wxFileName filename(m_files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
//		if (d2d::FileType::isType(filepath, m_type))
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.fn_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			if (check_json_key(value, key, filepath.ToStdString()))
			{
				float old = value[key].asDouble();
				value[key] = (int)old * times;

				Json::StyledStreamWriter writer;
				std::locale::global(std::locale(""));
				std::ofstream fout(filepath.fn_str());
				std::locale::global(std::locale("C"));
				writer.write(fout, value);
				fout.close();
			}
		}
	}
}

}
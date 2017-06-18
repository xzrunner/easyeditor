#include "FormatTIDString.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <fstream>

namespace edb
{

std::string FormatTIDString::Command() const
{
	return "format-tidstring";
}

std::string FormatTIDString::Description() const
{
	return "format string for tid";
}

std::string FormatTIDString::Usage() const
{
	// format-json e:/test2/1001

	return Command() + " [dir path]";
}

int FormatTIDString::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}

void FormatTIDString::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());

		// read
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		std::vector<std::string> lines;
		std::string str;
		while (getline(fin, str)) {
			lines.push_back(str);
		}
		fin.close();

		// write
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));
		for (int i = 0, n = lines.size(); i < n; ++i){
			fout << lines[i] << "\\n";
		}
		fout.close();
	}
}

}
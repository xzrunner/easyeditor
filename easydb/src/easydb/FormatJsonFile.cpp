#include "FormatJsonFile.h"
#include "check_params.h"

#include <wx/wx.h>
#include <drag2d.h>

namespace edb
{

std::string FormatJsonFile::Command() const
{
	return "format-json";
}

std::string FormatJsonFile::Description() const
{
	return "format json file, load and store";
}

std::string FormatJsonFile::Usage() const
{
	// format-json e:/test2/1001

	return Command() + " [dir path]";
}

void FormatJsonFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void FormatJsonFile::Trigger(const std::string& dir) const
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();

		int pos = filepath.rfind('.');
		if (pos == -1) {
			continue;
		}

		std::string ext = filepath.substr(pos);
		if (ext == ".json")
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.fn_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

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
#include "FormatTIDString.h"
#include "check_params.h"

#include <wx/wx.h>
#include <drag2d.h>

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

void FormatTIDString::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void FormatTIDString::Trigger(const std::string& dir) const
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();

		// read
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.fn_str());
		std::locale::global(std::locale("C"));
		std::vector<std::string> lines;
		std::string str;
		while (getline(fin, str)) {
			lines.push_back(str);
		}
		fin.close();

		// write
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.fn_str());
		std::locale::global(std::locale("C"));
		for (int i = 0, n = lines.size(); i < n; ++i){
			fout << lines[i] << "\\n";
		}
		fout.close();
	}
}

}
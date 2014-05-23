#include "SetNameFromFile.h"
#include "check_params.h"

#include <wx/wx.h>
#include <drag2d.h>

namespace edb
{

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

	return Command() + " [dir path]";
}

void SetNameFromFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	AddNameFromFile(argv[2]);
}

void SetNameFromFile::AddNameFromFile(const std::string& dir) const
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.fn_str());
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
				std::ofstream fout(filepath.fn_str());
				writer.write(fout, value);
				fout.close();
			}
		}
	}
}

}
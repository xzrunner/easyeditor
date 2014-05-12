#include "FixAnimation.h"

#include <wx/wx.h>
#include <drag2d.h>

namespace edb
{

FixAnimation::FixAnimation(const std::string& dir)
{
	AddNameFromFile(dir);
}

void FixAnimation::AddNameFromFile(const std::string& dir) const
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
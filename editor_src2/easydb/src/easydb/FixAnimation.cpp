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
			std::ifstream fin(filepath.fn_str());
			reader.parse(fin, value);
			fin.close();

			std::string name = value["name"].asString();
			if (name.empty())
			{
				size_t begin = filepath.find_last_of('\\') + 1;
				size_t end = filepath.find_last_of('_');
				name = filepath.substr(begin, end - begin);
				value["name"] = name;

				Json::StyledStreamWriter writer;
				std::ofstream fout(filepath.fn_str());
				writer.write(fout, value);
				fout.close();
			}
		}
	}
}

}
#include "ReverseAnimation.h"

#include <wx/wx.h>
#include <drag2d.h>

namespace edb
{

ReverseAnimation::ReverseAnimation(const std::string& dir)
{
	Trigger(dir);
}

void ReverseAnimation::Trigger(const std::string& dir) const
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

			Json::Value reversed = value;
			for (int i = 0, n = value["layer"].size(); i < n; ++i)
			{
				for (int j = value["layer"][i]["frame"].size() - 1, index = 0; j >= 0; --j, ++index) 
				{
					reversed["layer"][i]["frame"][index] = value["layer"][i]["frame"][j];
					reversed["layer"][i]["frame"][index]["time"] = index + 1;
				}
			}

			Json::StyledStreamWriter writer;
			std::ofstream fout(filepath.fn_str());
			writer.write(fout, reversed);
			fout.close();
		}
	}
}

}
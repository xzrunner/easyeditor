#include "ReverseAnimation.h"
#include "check_params.h"

#include <wx/wx.h>


namespace edb
{

std::string ReverseAnimation::Command() const
{
	return "reverse-anim";
}

std::string ReverseAnimation::Description() const
{
	return "reverse anim's all frames";
}

std::string ReverseAnimation::Usage() const
{
	// reverse-anim E:\test2\1001

	return Command() + " [dir path]";
}

void ReverseAnimation::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void ReverseAnimation::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (ee::FileType::IsType(filepath, ee::FileType::e_anim))
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.fn_str());
			std::locale::global(std::locale("C"));
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
			std::locale::global(std::locale(""));
			std::ofstream fout(filepath.fn_str());
			std::locale::global(std::locale("C"));
			writer.write(fout, reversed);
			fout.close();
		}
	}
}

}
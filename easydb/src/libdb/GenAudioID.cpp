#include "GenAudioID.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <fstream>

namespace edb
{

std::string GenAudioID::Command() const
{
	return "gen-audio-id";
}

std::string GenAudioID::Description() const
{
	return "gen audio id";
}

std::string GenAudioID::Usage() const
{
	return Command() + " [src dir] [dst file]";
}

int GenAudioID::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void GenAudioID::Trigger(const std::string& src_dir, const std::string& dst_file)
{
	Json::Value val;

	// audio
	{
		wxArrayString files;
		ee::FileHelper::FetchAllFiles(src_dir + "\\audio", files);

		int idx = val.size();

		for (auto& file : files)
		{
			if (file.find("manga_") == std::string::npos) {
				continue;
			}
			Json::Value item;
			item["id"] = idx;
			item["name"] = "audio/" + ee::FileHelper::GetFilename(file.ToStdString());
			val[idx++] = item;
		}
		for (auto& file : files)
		{
			if (file.find("manga_") != std::string::npos) {
				continue;
			}
			Json::Value item;
			item["id"] = idx;
			item["name"] = "audio/" + ee::FileHelper::GetFilename(file.ToStdString());
			val[idx++] = item;
		}
	}

	// sound
	{
		wxArrayString files;
		ee::FileHelper::FetchAllFiles(src_dir + "\\sound", files);

		int idx = val.size();
		for (auto& file : files)
		{
			Json::Value item;
			item["id"] = idx;
			item["name"] = "sound/" + ee::FileHelper::GetFilename(file.ToStdString());
			val[idx++] = item;
		}
	}

	// music
	{
		wxArrayString files;
		ee::FileHelper::FetchAllFiles(src_dir + "\\music", files);

		int idx = val.size();
		for (auto& file : files)
		{
			Json::Value item;
			item["id"] = idx;
			item["name"] = "music/" + ee::FileHelper::GetFilename(file.ToStdString());
			val[idx++] = item;
		}
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(dst_file.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, val);
	fout.close();
}

}
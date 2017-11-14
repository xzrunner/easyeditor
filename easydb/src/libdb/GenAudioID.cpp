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

	LoadAudioData(src_dir + "//audio", val);
	LoadAudioData(src_dir + "//sound", val);
	LoadAudioData(src_dir + "//music", val);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(dst_file.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, val);
	fout.close();
}

void GenAudioID::LoadAudioData(const std::string& src_dir, Json::Value& dst)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);

	int idx = dst.size();
	for (auto& file : files) 
	{
		Json::Value item;
		item["id"] = idx;
		item["name"] = ee::FileHelper::GetFilename(file.ToStdString());
		dst[idx++] = item;
	}
}

}
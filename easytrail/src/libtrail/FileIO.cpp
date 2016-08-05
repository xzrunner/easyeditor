#include "FileIO.h"
#include "ToolbarPanel.h"

#include <ee/FileHelper.h>

#include <json/json.h>

#include <fstream>

namespace etrail
{

void FileIO::Store(const std::string& filepath, MotionTrail* mt, ToolbarPanel* toolbar)
{
	Json::Value value;
	std::string dir = ee::FileHelper::GetFileDir(filepath);
	toolbar->Store(value, dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const std::string& filepath, MotionTrail* mt, ToolbarPanel* toolbar, ee::LibraryPanel* library)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	toolbar->Load(value, dir);
}

MotionTrail* FileIO::LoadMT(const std::string& filepath)
{
	return NULL;
}

t2d_emitter_cfg* FileIO::LoadMTConfig(const std::string& filepath)
{
	return NULL;
}

}
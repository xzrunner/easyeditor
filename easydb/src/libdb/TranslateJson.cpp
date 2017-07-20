#include "TranslateJson.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SpriteIO.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

#include <wx/arrstr.h>

#include <fstream>

namespace edb
{

std::string TranslateJson::Command() const
{
	return "translate-json";
}

std::string TranslateJson::Description() const
{
	return "translate-json";	
}

std::string TranslateJson::Usage() const
{
	return Command() + " [dir path] [dx] [dy]";
}

int TranslateJson::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 5)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2], sm::vec2(atof(argv[3]), atof(argv[4])));

	return 0;
}

void TranslateJson::Trigger(const std::string& dir, const sm::vec2& offset) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_COMPLEX:
			TranslateComplex(filepath, offset);
			break;
		}
	}
}

void TranslateJson::TranslateComplex(const std::string& path, const sm::vec2& offset) const
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(path);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(path);

	for (int i = 0, n = value["sprite"].size(); i < n; ++i) {
		TranslateSprite(value["sprite"][i], offset);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void TranslateJson::TranslateSprite(Json::Value& spr_val, const sm::vec2& offset) const
{
	std::string dir = ee::FileHelper::GetFileDir(spr_val["filepath"].asString());

	ee::SpriteIO spr_io;
	spr_io.Load(spr_val, dir);

	spr_io.m_position += offset;

	spr_io.Store(spr_val, dir);
}

}
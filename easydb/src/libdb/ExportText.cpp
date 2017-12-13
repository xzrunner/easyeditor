#include "ExportText.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

namespace edb
{

std::string ExportText::Command() const
{
	return "export-text";
}

std::string ExportText::Description() const
{
	return "export text";
}

std::string ExportText::Usage() const
{
	std::string usage = Command() + " [src dir] [dst file]";
	return usage;
}

int ExportText::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void ExportText::Trigger(const std::string& src_dir, const std::string& dst_file)
{
	//Load(src_dir);
	//Export(dst_file);

	SetTID(src_dir);
}

void ExportText::Load(const std::string& dir)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (auto& file : files)
	{
		if (file.ToStdString().find("_text_complex.json") == std::string::npos) {
			continue;
		}

		auto filepath = file.ToStdString();
		auto filename = ee::FileHelper::GetFilename(file.ToStdString());

		std::string tid = filepath.substr(filepath.find("\\manga_") + 1);
		tid = tid.substr(0, tid.find("\\json"));
		tid += "_" + filename.substr(0, filename.find("_text_complex"));
		ee::StringHelper::ToUpper(tid);
	
		Text text;
		text.tid = tid;
		
		Json::Value val;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(file.ToStdString().c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, val);
		fin.close();

		std::string language = val["sprite"][0]["text"]["text"].asString();
		ee::StringHelper::ReplaceAll(language, "\n", "\\n");
		text.language.push_back(language);

		m_all_texts.push_back(text);
	}
}

void ExportText::Export(const std::string& file)
{
	std::ofstream fout(file.c_str());

	fout << "TID,¼òÌå,·±Ìå\n";
	for (auto& text : m_all_texts)
	{
		fout << text.tid << "," << text.language[0] << ",\n";
	}

	fout.close();
}

void ExportText::SetTID(const std::string& dir)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (auto& file : files)
	{
		if (file.ToStdString().find("_text_complex.json") == std::string::npos) {
			continue;
		}

		auto filepath = file.ToStdString();
		auto filename = ee::FileHelper::GetFilename(file.ToStdString());

		std::string tid = filepath.substr(filepath.find("\\manga_") + 1);
		tid = tid.substr(0, tid.find("\\json"));
		tid += "_" + filename.substr(0, filename.find("_text_complex"));
		ee::StringHelper::ToUpper(tid);

		Json::Value val;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(file.ToStdString().c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, val);
		fin.close();

		val["sprite"][0]["text"]["text"] = "";
		val["sprite"][0]["text"]["tid"] = tid;

		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(file.ToStdString().c_str());
		std::locale::global(std::locale("C"));
		writer.write(fout, val);
		fout.close();
	}
}

}
#include "OutlineToPolygon.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/FileType.h>
#include <ee/DummySymbol.h>

#include <easyimage.h>
#include <easyshape.h>

#include <gum/JsonSerializer.h>

namespace edb
{

std::string OutlineToPolygon::Command() const
{
	return "outline2poly";
}

std::string OutlineToPolygon::Description() const
{
	return "create polygon shape from outline";
}

std::string OutlineToPolygon::Usage() const
{
	// outline2poly E:\test2\image
	return Command() + " [dir]";
}

int OutlineToPolygon::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}


void OutlineToPolygon::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (!ee::FileType::IsType(filepath, ee::FILE_IMAGE)) {
			continue;
		}

		std::string outline_path = ee::FileHelper::GetFilenameAddTag(
			filepath, eimage::OUTLINE_FILE_TAG, "json");
		if (!ee::FileHelper::IsFileExist(filepath)) {
			continue;
		}

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(outline_path.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		std::vector<sm::vec2> vertices;
		gum::JsonSerializer::Load(value["normal"], vertices);
		if (vertices.empty()) {
			continue;
		}

		std::string shape_path = ee::FileHelper::GetFilenameAddTag(
			filepath, eshape::FILE_TAG, "json");

		eshape::PolygonShape poly(vertices);
		ee::DummySymbol bg(filepath);
		eshape::FileIO::StoreToFile(shape_path.c_str(), &poly, &bg);
	}
}

}
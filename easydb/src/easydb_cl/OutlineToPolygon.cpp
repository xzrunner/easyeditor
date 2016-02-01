#include "OutlineToPolygon.h"
#include "check_params.h"

#include <easyimage.h>
#include <easyshape.h>

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

void OutlineToPolygon::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}


void OutlineToPolygon::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (!ee::FileType::IsType(filepath, ee::FileType::e_image)) {
			continue;
		}

		wxString outline_path = ee::FileHelper::GetFilenameAddTag(
			filepath, eimage::OUTLINE_FILE_TAG, "json");
		if (!ee::FileHelper::IsFileExist(filepath)) {
			continue;
		}

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(outline_path.fn_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		std::vector<ee::Vector> vertices;
		ee::JsonSerializer::Load(value["normal"], vertices);
		if (vertices.empty()) {
			continue;
		}

		wxString shape_path = ee::FileHelper::GetFilenameAddTag(
			filepath, eshape::FILE_TAG, "json");

		std::vector<ee::Shape*> shapes;
		eshape::PolygonShape poly(vertices);
		shapes.push_back(&poly);

		ee::NullSymbol bg(filepath.ToStdString());

		eshape::FileIO::StoreToFile(shape_path.mb_str(), shapes, &bg);
	}
}

}
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
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
			continue;
		}

		wxString outline_path = d2d::FilenameTools::getFilenameAddTag(
			filepath, eimage::OUTLINE_FILE_TAG, "json");
		if (!d2d::FilenameTools::IsFileExist(filepath)) {
			continue;
		}

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(outline_path.fn_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		std::vector<d2d::Vector> vertices;
		d2d::JsonIO::Load(value["normal"], vertices);
		if (vertices.empty()) {
			continue;
		}

		wxString shape_path = d2d::FilenameTools::getFilenameAddTag(
			filepath, libshape::FILE_TAG, "json");

		std::vector<d2d::IShape*> shapes;
		libshape::PolygonShape poly(vertices);
		shapes.push_back(&poly);

		d2d::NullSymbol bg(filepath.ToStdString());

		libshape::FileIO::StoreToFile(shape_path.mb_str(), shapes, &bg);
	}
}

}
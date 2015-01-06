#include "OutlineToTriStrip.h"
#include "check_params.h"

#include <easyimage.h>

namespace edb
{

std::string OutlineToTriStrip::Command() const
{
	return "outline2strip";
}

std::string OutlineToTriStrip::Description() const
{
	return "create triangle strips from outline";
}

std::string OutlineToTriStrip::Usage() const
{
	// outline2strip E:\test2\image
	return Command() + " [dir]";
}

void OutlineToTriStrip::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}


void OutlineToTriStrip::Trigger(const std::string& dir) const
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
		if (!d2d::FilenameTools::isExist(filepath)) {
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
		d2d::JsonTools::load(value["normal"], vertices);
		if (vertices.empty()) {
			continue;
		}

		std::vector<d2d::Vector> tris;
		d2d::Triangulation::normal(vertices, tris);
		std::vector<std::vector<d2d::Vector> > strips;
//		d2d::Triangulation::strips(tris, strips);
		strips.push_back(tris);

		Json::Value value_out;
		for (int i = 0, n = strips.size(); i < n; ++i) {
			d2d::JsonTools::store(strips[i], value_out["strips"][i]);
		}

		wxString out_file = d2d::FilenameTools::getFilenameAddTag(filepath, 
			eimage::TRI_STRIP_FILE_TAG, "json");
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(out_file.fn_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value_out);
		fout.close();	
	}
}

}
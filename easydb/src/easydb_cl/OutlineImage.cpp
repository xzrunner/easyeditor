#include "OutlineImage.h"
#include "check_params.h"

#include <drag2d.h>
#include <easyimage.h>

namespace edb
{

std::string OutlineImage::Command() const
{
	return "outline";
}

std::string OutlineImage::Description() const
{
	return "create images' outline";
}

std::string OutlineImage::Usage() const
{
	// outline E:\test2\image
	return Command() + " [dir]";
}

void OutlineImage::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}


void OutlineImage::Trigger(const std::string& dir) const
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

		std::cout << i << " / " << n << " : " << filepath << "\n";

		d2d::Image* image = d2d::ImageMgr::Instance()->getItem(filepath);

		eimage::ExtractOutlineRaw raw(*image);
		raw.CreateBorderLineAndMerge();
		eimage::ExtractOutlineFine fine(raw.GetBorderLine(), raw.GetBorderLineMerged());
		fine.Trigger(0.04f, 0.2f);

		Json::Value value;
		d2d::Vector offset(-0.5f*image->originWidth(), -0.5f*image->originHeight());
		std::vector<d2d::Vector> vertices(fine.GetResult());

		float src_area = image->clipWidth() * image->clipHeight();
		float dst_area = d2d::Math::GetPolygonArea(vertices);
		if (dst_area < src_area * 0.95f)
		{
			for (int i = 0, n = vertices.size(); i < n; ++i) {
				vertices[i] += offset;
			}
			d2d::JsonTools::store(vertices, value["normal"]);

			wxString out_file = d2d::FilenameTools::getFilenameAddTag(filepath, 
				eimage::OUTLINE_FILE_TAG, "json");
			Json::StyledStreamWriter writer;
			std::locale::global(std::locale(""));
			std::ofstream fout(out_file.fn_str());
			std::locale::global(std::locale("C"));	
			writer.write(fout, value);
			fout.close();	
		}

		image->Release();
	}
}

}
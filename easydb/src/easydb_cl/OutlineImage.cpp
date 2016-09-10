#include "OutlineImage.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/Image.h>
#include <ee/Math2D.h>

#include <easyimage.h>
#include <gum/JsonSerializer.h>

#include <wx/arrstr.h>

#include <fstream>

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

int OutlineImage::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}


void OutlineImage::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (!ee::FileType::IsType(filepath, ee::FileType::e_image)) {
			continue;
		}

		std::cout << i << " / " << n << " : " << filepath << "\n";

		ee::Image* image = ee::ImageMgr::Instance()->GetItem(filepath);

		eimage::ExtractOutlineRaw raw(*image);
		raw.CreateBorderLineAndMerge();
		eimage::ExtractOutlineFine fine(raw.GetBorderLine(), raw.GetBorderLineMerged());
		fine.Trigger(0.04f, 0.2f);

		Json::Value value;
		sm::vec2 offset(-0.5f*image->GetOriginWidth(), -0.5f*image->GetOriginHeight());
		std::vector<sm::vec2> vertices(fine.GetResult());

		float src_area = image->GetClippedWidth() * image->GetClippedHeight();
		float dst_area = ee::Math2D::GetPolygonArea(vertices);
		if (dst_area < src_area * 0.95f)
		{
			for (int i = 0, n = vertices.size(); i < n; ++i) {
				vertices[i] += offset;
			}
			gum::JsonSerializer::Store(vertices, value["normal"]);

			std::string out_file = ee::FileHelper::GetFilenameAddTag(filepath, 
				eimage::OUTLINE_FILE_TAG, "json");
			Json::StyledStreamWriter writer;
			std::locale::global(std::locale(""));
			std::ofstream fout(out_file.c_str());
			std::locale::global(std::locale("C"));	
			writer.write(fout, value);
			fout.close();	
		}

		image->RemoveReference();
	}
}

}
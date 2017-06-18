#include "OutlineImage.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/Image.h>
#include <ee/Math2D.h>
#include <ee/SymbolFile.h>
#include <ee/ImageData.h>

#include <easyimage.h>

#include <sprite2/SymType.h>
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
		if (ee::SymbolFile::Instance()->Type(filepath) != s2::SYM_IMAGE) {
			continue;
		}

		std::cout << i << " / " << n << " : " << filepath << "\n";

		ee::Image* image = ee::ImageMgr::Instance()->GetItem(filepath);
		ee::ImageData* img_data = ee::ImageDataMgr::Instance()->GetItem(filepath);

		eimage::ExtractOutlineRaw raw(img_data->GetPixelData(), img_data->GetWidth(), img_data->GetHeight());
		raw.CreateBorderLineAndMerge();
		eimage::ExtractOutlineFine fine(raw.GetBorderLine(), raw.GetBorderLineMerged());
		fine.Trigger(0.04f, 0.2f);

		Json::Value value;
		sm::vec2 offset;
		offset.x = -0.5f * image->GetOriginSize().x;
		offset.y = -0.5f * image->GetOriginSize().y;

		std::vector<sm::vec2> vertices(fine.GetResult());

		float src_area = image->GetClippedRegion().Width() * image->GetClippedRegion().Height();
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

		img_data->RemoveReference();
		image->RemoveReference();
	}
}

}
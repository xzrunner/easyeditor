#include "RotateTrimImage.h"
#include "check_params.h"
#include "utility.h"

#include <ee/Snapshoot.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/ImageSymbol.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>
#include <ee/Image.h>
#include <ee/Math2D.h>
#include <ee/MinBoundingBox.h>
#include <ee/EE_ShaderLab.h>

#include <SM_Calc.h>

#include <easyimage.h>

#include <wx/arrstr.h>

#include <fstream>

namespace edb
{

static const char* OUTPUT_FILE = "rotate-trim";

std::string RotateTrimImage::Command() const
{
	return "rotate-trim";
}

std::string RotateTrimImage::Description() const
{
	return "rotate and trim image, clip blank part";
}

std::string RotateTrimImage::Usage() const
{
	return Command() + " [dir path]";
}

int RotateTrimImage::Run(int argc, char *argv[])
{
	// rotate-trim e:/test2/1001

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	ee::Snapshoot ss;

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool ori_alpha_cfg = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	RotateTrim(ss, argv[2]);

	data.open_image_edge_clip = ori_clip_cfg;
	data.pre_multi_alpha = ori_alpha_cfg;

	return 0;
}

const char* RotateTrimImage::GetOutputFileName()
{
	return OUTPUT_FILE;
}

void RotateTrimImage::RotateTrim(ee::Snapshoot& ss, const std::string& dir)
{
	std::string output_file = dir + "\\" + OUTPUT_FILE;
	std::locale::global(std::locale(""));	
	std::ofstream fout(output_file.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	
	if (fout.fail()) {
		std::cout << "Can't open output file. \n";
		return;
	}

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		std::cout << i << " / " << n << " : " << filepath << "\n";

		if (ee::FileType::IsType(filepath, ee::FILE_IMAGE))
		{
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);

			ee::Image* image = static_cast<ee::ImageSymbol*>(sym)->GetImage();
			int width, height;
			sm::vec2 center;
			float angle;
			bool success = GetRotateTrimInfo(image, width, height, center, angle);
			if (!success || angle == 0) {
				image->RemoveReference();
				continue;
			}

			ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
			spr->SetPosition(center);
			spr->SetAngle(angle);
			ss.DrawSprite(spr, true, width, height);

			spr->RemoveReference();
			sym->RemoveReference();

			//std::string dir = ee::FileHelper::getFileDir(filepath);
			//std::string name = ee::FileHelper::getFilename(filepath);
 		//	std::string outpath = dir + "\\test_" + name + ".png";
 		//	ss.SaveToFile(outpath, width, height);

			ss.SaveToFile(filepath, width, height);

			// output info
			std::string path = ee::FileHelper::GetRelativePath(dir, filepath);
			fout << path << " " << center.x << " " << center.y << " " << angle << "\n";
		}
	}

	fout.close();
}

bool RotateTrimImage::GetRotateTrimInfo(const ee::Image* image, int& width, int& height,
										sm::vec2& center, float& angle) const
{
	eimage::ExtractOutlineRaw raw(*image);
	raw.CreateBorderLineAndMerge();
	if (raw.GetBorderLine().empty()) {
		return false;
	}
	raw.CreateBorderConvexHull();

	sm::vec2 bound[4];
	bool is_rotate = ee::MinBoundingBox::Do(raw.GetConvexHull(), bound);

	center = (bound[0] + bound[2]) * 0.5f;
	center.x -= image->GetOriginWidth() * 0.5f;
	center.y -= image->GetOriginHeight() * 0.5f;

	center = -center;

	if (is_rotate) {
		float left = FLT_MAX;
		int left_idx;
		for (int i = 0; i < 4; ++i) {
			if (bound[i].x < left) {
				left = bound[i].x;
				left_idx = i;
			}
		}

		const sm::vec2& s = bound[left_idx];
		const sm::vec2& e = bound[left_idx == 3 ? 0 : left_idx + 1];
		sm::vec2 right = s;
		right.x += 1;
		angle = -sm::get_angle(s, e, right);
		center = sm::rotate_vector(center, angle);

		width = std::ceil(sm::dis_pos_to_pos(s, e));
		height = std::ceil(sm::dis_pos_to_pos(e, bound[(left_idx+2)%4]));
	} else {
		angle = 0;
	}

	return true;
}

}
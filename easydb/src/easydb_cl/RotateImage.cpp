#include "RotateImage.h"
#include "check_params.h"
#include "utility.h"

#include <ee/Snapshoot.h>
#include <ee/SettingData.h>
#include <ee/FileHelper.h>
#include <ee/Config.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/Rect.h>
#include <ee/Math2D.h>
#include <ee/Sprite.h>
#include <ee/StringHelper.h>
#include <ee/EE_ShaderLab.h>

#include <easyimage.h>

namespace edb
{

std::string RotateImage::Command() const
{
	return "rotate-image";
}

std::string RotateImage::Description() const
{
	return "rotate image";
}

std::string RotateImage::Usage() const
{
	return Command() + " [src dir] [dst dir]";
}

int RotateImage::Run(int argc, char *argv[])
{
	// rotate-image e:/test2/1001 e:/test2/1001

	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

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

	Rotate(ss, argv[2], argv[3]);

	data.open_image_edge_clip = ori_clip_cfg;
	data.pre_multi_alpha = ori_alpha_cfg;

	return 0;
}

void RotateImage::Rotate(ee::Snapshoot& ss, const std::string& src_dir, const std::string& dst_dir)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_image))
		{
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
			ee::Rect r = symbol->GetSize();
			for (int deg = 10; deg <= 90; deg += 10) {
				float rad = deg * ee::TRANS_DEG_TO_RAD;
				sprite->SetTransform(sprite->GetPosition(), rad);
				int width = ee::Math2D::RotateVector(ee::Vector(r.xmax, r.ymax), -rad).x * 2;
				int height = ee::Math2D::RotateVector(ee::Vector(r.xmin, r.ymax), -rad).y * 2;
				ss.DrawSprite(sprite, true, width, height);

				std::string name = ee::FileHelper::GetFilename(filepath);
				std::string outpath = ee::StringHelper::Format("%s\\%s_%d.png", dst_dir.c_str(), name.c_str(), deg);
				ss.SaveToFile(outpath, width, height);
			}

			sprite->Release();
			symbol->Release();
		}
	}
}

}
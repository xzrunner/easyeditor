#include "RotateImage.h"
#include "check_params.h"
#include "utility.h"

#include <ee/Snapshoot.h>
#include <ee/SettingData.h>
#include <ee/FileHelper.h>
#include <ee/Config.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>
#include <ee/StringHelper.h>
#include <ee/EE_ShaderLab.h>
#include <ee/SymbolFile.h>

#include <easyimage.h>

#include <SM_Calc.h>
#include <sprite2/SymType.h>

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
		if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_IMAGE)
		{
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
			sm::rect r = sym->GetBounding();
			for (int deg = 10; deg <= 90; deg += 10) {
				float rad = deg * SM_DEG_TO_RAD;
				spr->SetAngle(rad);
				int width = sm::rotate_vector(sm::vec2(r.xmax, r.ymax), -rad).x * 2;
				int height = sm::rotate_vector(sm::vec2(r.xmin, r.ymax), -rad).y * 2;
				ss.DrawSprite(spr, true, width, height);

				std::string name = ee::FileHelper::GetFilename(filepath);
				std::string outpath = ee::StringHelper::Format("%s\\%s_%d.png", dst_dir.c_str(), name.c_str(), deg);
				ss.SaveToFile(outpath, width, height);
			}

			spr->RemoveReference();
			sym->RemoveReference();
		}
	}
}

}
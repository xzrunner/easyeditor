#include "ScaleImage.h"
#include "check_params.h"
#include "utility.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/Sprite.h>
#include <ee/Symbol.h>
#include <ee/SpriteFactory.h>
#include <ee/SymbolFile.h>

#include <easyimage.h>

#include <sprite2/SymType.h>
#include <sprite2/DrawRT.h>
#include <gum/Config.h>

#include <wx/arrstr.h>

namespace edb
{

std::string ScaleImage::Command() const
{
	return "scale-image";
}

std::string ScaleImage::Description() const
{
	return "scale image";
}

std::string ScaleImage::Usage() const
{
	std::string cmd0 = Command() + " [dir] [scale]";
	std::string cmd1 = Command() + " [src] [dst] [scale]";
	return cmd0 + " or " + cmd1;
}

int ScaleImage::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (argc == 4 && !check_folder(argv[2])) return -1;

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;

	gum::Config* cfg = gum::Config::Instance();
	bool ori_alpha_cfg = cfg->GetPreMulAlpha();
	cfg->SetPreMulAlpha(false);

	if (argc == 4) {
		Scale(argv[2], atof(argv[3]));
	} else if (argc == 5) {
		Scale(argv[2], argv[3], atof(argv[4]));
	}

	data.open_image_edge_clip = ori_clip_cfg;
	cfg->SetPreMulAlpha(ori_alpha_cfg);

	return 0;
}

void ScaleImage::Scale(const std::string& dir, float scale)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i) {
		Scale(files[i].ToStdString(), files[i].ToStdString(), scale);
	}
}

void ScaleImage::Scale(const std::string& src, const std::string& dst, float scale)
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(src);
	if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_IMAGE)
	{
		ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		sm::rect r = sym->GetBounding();

		ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
		spr->SetScale(sm::vec2(scale, scale));

		s2::DrawRT rt;
		const sm::vec2& sz = r.Size();
		int width = sz.x * scale,
			height = sz.y * scale;
		rt.Draw(spr, true, width, height);
		rt.StoreToFile(dst, width, height);

		spr->RemoveReference();
		sym->RemoveReference();
	}
}

}
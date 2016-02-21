#include <gl/glew.h>

#include "ScaleImage.h"
#include "check_params.h"

#include <ee/ShaderMgr.h>
#include <ee/SettingData.h>
#include <ee/Snapshoot.h>
#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/Rect.h>
#include <ee/Sprite.h>
#include <ee/Symbol.h>
#include <ee/SpriteFactory.h>

#include <glfw.h>
#include <wx/arrstr.h>

#include <easyimage.h>

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

void ScaleImage::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return;
	if (argc == 4 && !check_folder(argv[2])) return;

	glfwInit();
	if(!glfwOpenWindow(100, 100, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return;
	}

	if (glewInit() != GLEW_OK) {
		return;
	}

	ee::ShaderMgr::Instance()->reload();

	ee::Snapshoot ss;

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool ori_alpha_cfg = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	if (argc == 4) {
		Scale(ss, argv[2], atof(argv[3]));
	} else if (argc == 5) {
		Scale(ss, argv[2], argv[3], atof(argv[4]));
	}

	data.open_image_edge_clip = ori_clip_cfg;
	data.pre_multi_alpha = ori_alpha_cfg;
}

void ScaleImage::Scale(ee::Snapshoot& ss, const std::string& dir, float scale)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i) {
		Scale(ss, files[i].ToStdString(), files[i].ToStdString(), scale);
	}
}

void ScaleImage::Scale(ee::Snapshoot& ss, const std::string& src, const std::string& dst, float scale)
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(src);
	if (ee::FileType::IsType(filepath, ee::FileType::e_image))
	{
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		ee::Rect r = symbol->GetSize();

		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		sprite->SetScale(ee::Vector(scale, scale));

		int width = r.Width() * scale,
			height = r.Height() * scale;
		ss.DrawSprite(sprite, true, width, height);

		sprite->Release();
		symbol->Release();

		ss.SaveToFile(dst, width, height);
	}
}

}
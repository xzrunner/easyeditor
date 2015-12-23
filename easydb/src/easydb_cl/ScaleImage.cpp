#include <gl/glew.h>

#include "ScaleImage.h"
#include "check_params.h"

#include <glfw.h>

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

	d2d::ShaderMgr::Instance()->reload();

	d2d::Snapshoot ss;

	d2d::SettingData& data = d2d::Config::Instance()->GetSettings();
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

void ScaleImage::Scale(d2d::Snapshoot& ss, const std::string& dir, float scale)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i) {
		Scale(ss, files[i].ToStdString(), files[i].ToStdString(), scale);
	}
}

void ScaleImage::Scale(d2d::Snapshoot& ss, const std::string& src, const std::string& dst, float scale)
{
	wxFileName filename(src);
	filename.Normalize();
	std::string filepath = filename.GetFullPath().ToStdString();
	if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
	{
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
		d2d::Rect r = symbol->GetSize();

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->SetScale(d2d::Vector(scale, scale));

		int width = r.xLength() * scale,
			height = r.yLength() * scale;
		ss.DrawSprite(sprite, true, width, height);

		sprite->Release();
		symbol->Release();

		ss.SaveToFile(dst, width, height);
	}
}

}
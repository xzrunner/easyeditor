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
	return Command() + " [dir] [scale]";
}

void ScaleImage::Run(int argc, char *argv[])
{
	// scale-image e:/test2/1001

	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2], atof(argv[3]));
}

void ScaleImage::Trigger(const std::string& dir, float scale)
{
	glfwInit();
	if(!glfwOpenWindow(800, 600, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return;
	}

	d2d::ShaderMgr::Instance()->reload();

	d2d::Snapshoot ss;

	d2d::SettingData& data = d2d::Config::Instance()->GetSettings();
	bool old_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			d2d::Rect r = symbol->GetSize();

			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			sprite->SetScale(scale, scale);

			int width = r.xLength() * scale,
				height = r.yLength() * scale;
			ss.DrawSprite(sprite, true, width, height);

			sprite->Release();
			symbol->Release();

			ss.SaveToFile(filepath, width, height);
		}
	}

	data.open_image_edge_clip = old_cfg;
}

}
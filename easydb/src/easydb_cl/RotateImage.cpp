#include "RotateImage.h"
#include "check_params.h"

#include <glfw.h>
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

void RotateImage::Run(int argc, char *argv[])
{
	// rotate-image e:/test2/1001 e:/test2/1001

	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[3])) return;

	Trigger(argv[2], argv[3]);
}

void RotateImage::Trigger(const std::string& src_dir, const std::string& dst_dir)
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
	d2d::FilenameTools::fetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			d2d::Rect r = symbol->GetSize();
			for (int deg = 10; deg <= 90; deg += 10) {
				float rad = deg * d2d::TRANS_DEG_TO_RAD;
				sprite->setTransform(sprite->getPosition(), rad);
				int width = d2d::Math::rotateVector(d2d::Vector(r.xMax, r.yMax), -rad).x * 2;
				int height = d2d::Math::rotateVector(d2d::Vector(r.xMin, r.yMax), -rad).y * 2;
				ss.DrawSprite(sprite, true, width, height);

				wxString name = d2d::FilenameTools::getFilename(filepath);
				wxString outpath;
				outpath.Printf("%s\\%s_%d.png", dst_dir, name, deg);
				ss.SaveToFile(outpath.ToStdString(), width, height);
			}

			sprite->Release();
			symbol->Release();
		}
	}

	data.open_image_edge_clip = old_cfg;
}

}
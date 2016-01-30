#include <gl/glew.h>

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

	Rotate(ss, argv[2], argv[3]);

	data.open_image_edge_clip = ori_clip_cfg;
	data.pre_multi_alpha = ori_alpha_cfg;
}

void RotateImage::Rotate(d2d::Snapshoot& ss, const std::string& src_dir, const std::string& dst_dir)
{
	wxArrayString files;
	d2d::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();
		if (d2d::FileType::IsType(filepath, d2d::FileType::e_image))
		{
			d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			d2d::Sprite* sprite = d2d::SpriteFactory::Instance()->Create(symbol);
			d2d::Rect r = symbol->GetSize();
			for (int deg = 10; deg <= 90; deg += 10) {
				float rad = deg * d2d::TRANS_DEG_TO_RAD;
				sprite->SetTransform(sprite->GetPosition(), rad);
				int width = d2d::Math2D::RotateVector(d2d::Vector(r.xmax, r.ymax), -rad).x * 2;
				int height = d2d::Math2D::RotateVector(d2d::Vector(r.xmin, r.ymax), -rad).y * 2;
				ss.DrawSprite(sprite, true, width, height);

				wxString name = d2d::FileHelper::GetFilename(filepath);
				wxString outpath;
				outpath.Printf("%s\\%s_%d.png", dst_dir, name, deg);
				ss.SaveToFile(outpath.ToStdString(), width, height);
			}

			sprite->Release();
			symbol->Release();
		}
	}
}

}
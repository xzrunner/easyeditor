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

	ee::ShaderMgr::Instance()->reload();

	ee::Snapshoot ss;

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool ori_alpha_cfg = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	Rotate(ss, argv[2], argv[3]);

	data.open_image_edge_clip = ori_clip_cfg;
	data.pre_multi_alpha = ori_alpha_cfg;
}

void RotateImage::Rotate(ee::Snapshoot& ss, const std::string& src_dir, const std::string& dst_dir)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();
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

				wxString name = ee::FileHelper::GetFilename(filepath);
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
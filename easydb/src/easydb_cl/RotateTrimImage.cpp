#include "RotateTrimImage.h"
#include "check_params.h"

#include <glfw.h>
#include <easyimage.h>

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

void RotateTrimImage::Run(int argc, char *argv[])
{
	// rotate-trim e:/test2/1001

	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

const char* RotateTrimImage::GetOutputFileName()
{
	return OUTPUT_FILE;
}

void RotateTrimImage::Trigger(const std::string& dir)
{
	glfwInit();
	if(!glfwOpenWindow(800, 600, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return;
	}

	d2d::ShaderMgr::Instance()->reload();

	d2d::Snapshoot ss;

	std::string output_file = dir + "\\" + OUTPUT_FILE;
	std::ofstream fout(output_file.c_str(), std::ios::binary);
	if (fout.fail()) {
		std::cout << "Can't open output file. \n";
		return;
	}

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();

		std::cout << i << " / " << n << " : " << filepath << "\n";

		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);

			d2d::Image* image = static_cast<d2d::ImageSymbol*>(symbol)->getImage();
			int width, height;
			d2d::Vector center;
			float angle;
			bool success = GetRotateTrimInfo(image, width, height, center, angle);
			if (!success || angle == 0) {
				image->Release();
				continue;
			}

			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			sprite->setTransform(center, angle);
			ss.DrawSprite(sprite, true, width, height);

			sprite->Release();
			symbol->Release();

			//wxString dir = d2d::FilenameTools::getFileDir(filepath);
			//wxString name = d2d::FilenameTools::getFilename(filepath);
 		//	wxString outpath = dir + "\\test_" + name + ".png";
 		//	ss.SaveToFile(outpath.ToStdString(), width, height);

			ss.SaveToFile(filepath.ToStdString(), width, height);

			// output info
			wxString path = d2d::FilenameTools::getRelativePath(dir, filepath);
			fout << path << " " << center.x << " " << center.y << " " << angle << "\n";
		}
	}

	fout.close();
}

bool RotateTrimImage::GetRotateTrimInfo(const d2d::Image* image, int& width, int& height,
										d2d::Vector& center, float& angle) const
{
	eimage::ExtractOutlineRaw raw(*image);
	raw.CreateBorderLineAndMerge();
	if (raw.GetBorderLine().empty()) {
		return false;
	}
	raw.CreateBorderConvexHull();

	d2d::Vector bound[4];
	bool is_rotate = d2d::MinBoundingBox::Implement(raw.GetConvexHull(), bound);

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

		const d2d::Vector& s = bound[left_idx];
		const d2d::Vector& e = bound[left_idx == 3 ? 0 : left_idx + 1];
		d2d::Vector right = s;
		right.x += 1;
		angle = -d2d::Math::getAngle(s, e, right);
		center = d2d::Math::rotateVector(center, angle);

		width = std::ceil(d2d::Math::getDistance(s, e));
		height = std::ceil(d2d::Math::getDistance(e, bound[(left_idx+2)%4]));
	} else {
		angle = 0;
	}

	return true;
}

}
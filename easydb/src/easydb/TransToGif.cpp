#include "TransToGif.h"
#include "check_params.h"

#include <glfw.h>
#include <drag2d.h>
#include <easyanim.h>
#include <easyimage.h>

namespace edb
{

std::string TransToGif::Command() const
{
	return "togif";
}

std::string TransToGif::Description() const
{
	return "translate anims to gif files";
}

std::string TransToGif::Usage() const
{
	// togif D:\projects\ejoy\coco-tools\sg_characters_new\data\json\2003daoke\attack1\1 E:\gif

	return Command() + " [src path] [dst path]";
}

void TransToGif::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[3])) return;

	Trigger(argv[2], argv[3]);
}

void TransToGif::Trigger(const std::string& srcdir, const std::string& dstdir) const
{
	glfwInit();
	if(!glfwOpenWindow(800, 600, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return;
	}
	d2d::Snapshoot ss;

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(srcdir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.fn_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			std::string name = value["name"].asString();
			if (name.empty()) {
				continue;
			}
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			eanim::Symbol* anim = static_cast<eanim::Symbol*>(symbol);

			int max_frame = anim->getMaxFrameIndex();
			int width = symbol->getSize().xLength();
			int height = symbol->getSize().yLength();
			AnimatedGifSaver saver(width, height);
			for (int i = 0; i < max_frame; ++i)
			{
				anim->setFrameIndex(i + 1);
				uint8_t* rgba = ss.outputToMemory(symbol, true);

				uint8_t* rgb = eimage::RGBA2RGB(rgba, width, height, true);
				saver.AddFrame(rgb, 1.0f / anim->getFPS());
				delete[] rgba;
				delete[] rgb;
			}
			anim->setFrameIndex(0);
			std::string filename = dstdir + "//" + name + ".gif";
			saver.Save(filename.c_str());

			symbol->Release();
		}
	}
}

}
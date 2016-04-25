#include "TransToGif.h"
#include "check_params.h"
#include "utility.h"

#include <ee/Snapshoot.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/AnimatedGifSaver.h>
#include <ee/EE_ShaderLab.h>

#include <fstream>

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

int TransToGif::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	ee::Snapshoot ss;

	Run(ss, argv[2], argv[3]);

	return 0;
}

void TransToGif::Run(ee::Snapshoot& ss, const std::string& srcdir, const std::string& dstdir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(srcdir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_anim))
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.c_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			std::string name = value["name"].asString();
			if (name.empty()) {
				continue;
			}
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			eanim::Symbol* anim = static_cast<eanim::Symbol*>(symbol);

			int max_frame = anim->getMaxFrameIndex();
			int width = symbol->GetSize().Width();
			int height = symbol->GetSize().Height();
			AnimatedGifSaver saver(width, height);
			for (int i = 0; i < max_frame; ++i)
			{
				anim->setFrameIndex(i + 1);
				uint8_t* rgba = ss.OutputToMemory(symbol, true);

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
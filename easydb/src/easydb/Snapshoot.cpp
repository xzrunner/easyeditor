#include <gl/glew.h>

#include "Snapshoot.h"
#include "check_params.h"

#include <ee/Snapshoot.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/Symbol.h>
#include <ee/EE_ShaderLab.h>

#include <glfw.h>
#include <fstream>
#include <wx/arrstr.h>

namespace edb
{

std::string Snapshoot::Command() const
{
	return "snapshoot";
}

std::string Snapshoot::Description() const
{
	return "get snapshoot from json files";
}

std::string Snapshoot::Usage() const
{
	// snapshoot E:\test2\image E:\test2\image

	return Command() + " [src path] [dst path]";
}

int Snapshoot::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

	glfwInit();
	if(!glfwOpenWindow(100, 100, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return -2;
	}

	if (glewInit() != GLEW_OK) {
		return -2;
	}

	ee::ShaderLab::Instance()->Init();

	ee::Snapshoot ss;

	Run(ss, argv[2], argv[3]);

	return 0;
}

void Snapshoot::Run(ee::Snapshoot& ss, const std::string& srcdir, const std::string& dstdir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(srcdir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex) || 
			ee::FileType::IsType(filepath, ee::FileType::e_anim))
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
			std::string filename = dstdir + "//" + name;
			ss.OutputToImageFile(symbol, filename);
			symbol->Release();
		}
	}
}

}
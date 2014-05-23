#include "Snapshoot.h"
#include "check_params.h"

#include <glfw.h>
#include <drag2d.h>

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

void Snapshoot::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[3])) return;

	Trigger(argv[2], argv[3]);
}


void Snapshoot::Trigger(const std::string& srcdir, const std::string& dstdir) const
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
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex) || 
			d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
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
			std::string filename = dstdir + "//" + name;
			ss.outputToImageFile(symbol, filename);
			symbol->release();
		}
	}
}

}
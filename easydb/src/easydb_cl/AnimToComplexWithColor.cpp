#include "AnimToComplexWithColor.h"
#include "check_params.h"

#include <easycomplex.h>

namespace edb
{

std::string AnimToComplexWithColor::Command() const
{
	return "anim2complex-with-color";
}

std::string AnimToComplexWithColor::Description() const
{
	return "create complex file with anim file and set color";
}

std::string AnimToComplexWithColor::Usage() const
{
	// anim2complex-with-color e:/test2/trans.txt
	std::string usage = Command() + " [cfg filepath]";
	return usage;
}

void AnimToComplexWithColor::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Run(argv[2]);
}

void AnimToComplexWithColor::Run(const std::string& cfg_filepath)
{
	std::string dir = d2d::FilenameTools::getFileDir(cfg_filepath);

	std::locale::global(std::locale(""));
	std::ifstream fin(cfg_filepath.c_str());
	std::locale::global(std::locale("C"));

	std::string line;
	d2d::Colori last_col;
	while (std::getline(fin, line)) 
	{
		std::stringstream ss(line);

		std::string filepath;
		d2d::Colori col;
		ss >> filepath >> col.r >> col.g >> col.b;
		if (col.r == 0 && col.g == 0 && col.b == 0) {
			col = last_col;
		} else {
			last_col = col;
		}
		//////////////////////////////////////////////////////////////////////////
		ecomplex::Symbol* symbol = new ecomplex::Symbol;
		d2d::ISprite* sprite = new d2d::NullSprite(new d2d::NullSymbol(dir + "\\" + filepath + ".json"));

		d2d::Colori rgb = d2d::hsl2rgb(col.r, col.g, col.b);
		sprite->g_trans = d2d::Colorf(rgb.r / 255.0f, rgb.g / 255.0f, rgb.b / 255.0f, 1);
		symbol->m_sprites.push_back(sprite);

		std::string output = filepath.substr(0, filepath.find_last_of('_'));
		output = dir + "\\" + output + "_blue_complex.json";
		ecomplex::FileSaver::store(output.c_str(), symbol);

		delete symbol;
		//////////////////////////////////////////////////////////////////////////
	}
	fin.close();
}

}
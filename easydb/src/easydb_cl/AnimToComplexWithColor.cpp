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

	Trans trans;
	std::string line;
	while (std::getline(fin, line)) 
	{
		std::string filepath = ParserTrans(line, trans);

		ecomplex::Symbol* symbol = new ecomplex::Symbol;
		d2d::ISprite* sprite = new d2d::NullSprite(new d2d::NullSymbol(dir + "\\" + filepath + ".json"));
		TransSpr(trans, sprite);
		symbol->m_sprites.push_back(sprite);

		std::string output = filepath.substr(0, filepath.find_last_of('_'));
		int pos = output.find_last_of('_');
		std::string angle = output.substr(pos + 1);
		output = output.substr(0, pos);

		printf("line %s, ext %s \n", line.c_str(), trans.ext.c_str());

		output = dir + "\\" + output + "_" + trans.ext + "_" + angle + "_complex.json";
		ecomplex::FileStorer::Store(output.c_str(), symbol);

		delete symbol;
	}
	fin.close();
}

std::string AnimToComplexWithColor::ParserTrans(const std::string& str, struct Trans& t)
{
	std::vector<std::string> token;
	d2d::StringTools::Split(str, " ", token);

	if (token.size() == 0) {
		return "";
	} else if (token.size() == 1) {
		return token[0];
	}

	int ori_type = t.type;
	t.type = 0;
	bool set_color = false;

	int ptr = 1;
	while (ptr < token.size()) 
	{
		if (token[ptr] == "r" ||
			token[ptr] == "g" ||
			token[ptr] == "b") 
		{
			set_color = true;
			d2d::Colori* col;
			if (token[ptr] == "r") {
				col = &t.col_r;
				t.type |= CC_R;
			} else if (token[ptr] == "g") {
				col = &t.col_g;
				t.type |= CC_G;
			} else {
				col = &t.col_b;
				t.type |= CC_B;
			}
			++ptr;
			d2d::StringTools::FromString(token[ptr++], col->r);
			d2d::StringTools::FromString(token[ptr++], col->g);
			d2d::StringTools::FromString(token[ptr++], col->b);
		} 
		else 
		{
			t.ext = token[ptr++];
		}
	}

	if (!set_color) {
		t.type = ori_type;
	}

	return token[0];
}

void AnimToComplexWithColor::TransSpr(const Trans& t, d2d::ISprite* spr)
{
	if (t.type & CC_R) {
		d2d::Colori rgb = d2d::hsl2rgb(t.col_r.r, t.col_r.g, t.col_r.b);
		spr->color.r = d2d::Colorf(rgb.r / 255.0f, rgb.g / 255.0f, rgb.b / 255.0f, 1);
	}
	if (t.type & CC_G) {
		d2d::Colori rgb = d2d::hsl2rgb(t.col_g.r, t.col_g.g, t.col_g.b);
		spr->color.g = d2d::Colorf(rgb.r / 255.0f, rgb.g / 255.0f, rgb.b / 255.0f, 1);
	}
	if (t.type & CC_B) {
		d2d::Colori rgb = d2d::hsl2rgb(t.col_b.r, t.col_b.g, t.col_b.b);
		spr->color.b = d2d::Colorf(rgb.r / 255.0f, rgb.g / 255.0f, rgb.b / 255.0f, 1);
	}
}

}
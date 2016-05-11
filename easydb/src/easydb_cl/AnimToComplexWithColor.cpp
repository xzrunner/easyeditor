#include "AnimToComplexWithColor.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/NullSprite.h>
#include <ee/NullSymbol.h>
#include <ee/StringHelper.h>
#include <ee/convert_color.h>
#include <ee/RenderParams.h>

#include <easycomplex.h>

#include <fstream>

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

int AnimToComplexWithColor::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_file(argv[2])) return -1;

	Run(argv[2]);

	return 0;
}

void AnimToComplexWithColor::Run(const std::string& cfg_filepath)
{
	std::string dir = ee::FileHelper::GetFileDir(cfg_filepath);

	std::locale::global(std::locale(""));
	std::ifstream fin(cfg_filepath.c_str());
	std::locale::global(std::locale("C"));

	Trans trans;
	std::string line;
	while (std::getline(fin, line)) 
	{
		std::string filepath = ParserTrans(line, trans);

		ecomplex::Symbol* symbol = new ecomplex::Symbol;
		ee::Sprite* sprite = new ee::NullSprite(new ee::NullSymbol(dir + "\\" + filepath + ".json"));
		TransSpr(trans, sprite);
		symbol->m_sprites.push_back(sprite);

		std::string output = filepath.substr(0, filepath.find_last_of('_'));
		int pos = output.find_last_of('_');
		std::string angle = output.substr(pos + 1);
		output = output.substr(0, pos);

		output = dir + "\\" + output + "_" + trans.ext + "_" + angle + "_complex.json";
		ecomplex::FileStorer::Store(output.c_str(), symbol);

		delete symbol;
	}
	fin.close();
}

std::string AnimToComplexWithColor::ParserTrans(const std::string& str, struct Trans& t)
{
	std::vector<std::string> token;
	ee::StringHelper::Split(str, " ", token);

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
			ee::Colori* col;
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
			ee::StringHelper::FromString(token[ptr++], col->r);
			ee::StringHelper::FromString(token[ptr++], col->g);
			ee::StringHelper::FromString(token[ptr++], col->b);
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

void AnimToComplexWithColor::TransSpr(const Trans& t, ee::Sprite* spr)
{
	if (t.type & CC_R) {
		float r, g, b;
		ee::hsl2rgb(t.col_r.r / 255.0f, t.col_r.g / 255.0f, t.col_r.b / 255.0f, r, g, b);
		spr->GetColor().rmap.FromFloat(r, g, b);
	}
	if (t.type & CC_G) {
		float r, g, b;
		ee::hsl2rgb(t.col_g.r / 255.0f, t.col_g.g / 255.0f, t.col_g.b / 255.0f, r, g, b);
		spr->GetColor().gmap.FromFloat(r, g, b);
	}
	if (t.type & CC_B) {
		float r, g, b;
		ee::hsl2rgb(t.col_b.r / 255.0f, t.col_b.g / 255.0f, t.col_b.b / 255.0f, r, g, b);
		spr->GetColor().bmap.FromFloat(r, g, b);
	}
}

}
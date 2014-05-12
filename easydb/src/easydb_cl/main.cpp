#include <iostream>
#include <fstream>

#include "VerifyImages.h"
#include "VerifyJsons.h"
#include "FixImages.h"
#include "FixAnimation.h"
#include "FormatJsonFile.h"
#include "CopyFiles.h"
#include "Snapshoot.h"
#include "DistanceFieldFont.h"
#include "DFFParser.h"
#include "ReverseAnimation.h"
#include "PackParticle3D.h"
#include "ClipImages.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eanim::FILE_TAG, &anim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eanim::FILE_TAG, &anim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);
}

void verify(const std::string& dirpath, const std::string& op)
{
	if (op == "-images")
		edb::VerifyImages imgs(dirpath);
	else if (op == "-jsons")
		edb::VerifyJsons jsons(dirpath);
}

void fix(const std::string& path0, const std::string& path1, const std::string& op)
{
	if (op == "-images") {
		edb::FixImages fix(path0, path1);
	} else if (op == "-anim") {
		edb::FixAnimation fix(path0);
	}
}

void format(const std::string& dirpath, const std::string& op)
{
	if (op == "-jsons") {
		edb::FormatJsonFile::implement(dirpath);
	}
}

static void command(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "Need Command: [verify] ... !" << std::endl;
		std::cerr << "verify [folder path] (-images | -jsons)" << std::endl;
		return;
	}

	std::string cmd = argv[1];
	if (cmd == "verify")
	{
		if (argc < 3) {
			std::cerr << "Need Resources' folder path!" << std::endl;
			return;
		}

		std::string dirpath = argv[2];
		if (argc < 4) {
			std::cerr << "Need [-images] or [-jsons]!" << std::endl;
			return;
		}

		verify(dirpath, argv[3]);
		if (argc == 5)
			verify(dirpath, argv[4]);
	}
	else if (cmd == "fix")
	{
		if (argc < 5) {
			std::cerr << "Params: [path0] [path1] [-images] !" << std::endl;
			return;
		}

		fix(argv[2], argv[3], argv[4]);
	}
	else if (cmd == "reverse")
	{
		if (argc < 3) {
			std::cerr << "Params: [dir]!" << std::endl;
			return;
		}

		std::string dir = argv[2];
		edb::ReverseAnimation reverse(dir);
	}
	else if (cmd == "format")
	{
		if (argc < 4) {
			std::cerr << "Params: [-jsons] [dir] !" << std::endl;
			return;
		}

		format(argv[3], argv[2]);
	}
	else if (cmd == "copy")
	{
		if (argc < 5) {
			std::cerr << "Params: [src] [dst] [list]!" << std::endl;
			return;
		}

		std::string srcdir = argv[2];
		std::string dstdir = argv[3];
		std::string listfile = argv[4];

		std::set<std::string> names;
		std::locale::global(std::locale(""));
		std::ifstream fin(listfile.c_str());
		std::locale::global(std::locale("C"));
		if (fin.fail()) {
			std::cerr << "Can't open file " << listfile << std::endl;
			return;
		}
		std::string str;
		while (getline(fin, str)) {
			names.insert(str);
		}
		fin.close();

		edb::CopyFiles copy(srcdir, dstdir);
		copy.CopyByExportNames(names);
	}
	else if (cmd == "snapshoot")
	{
		if (argc < 4) {
			std::cerr << "Params: [src dir] [dst dir]!" << std::endl;
			return;
		}

		std::string srcdir = argv[2];
		std::string dstdir = argv[3];

		edb::Snapshoot::trigger(srcdir, dstdir);
	} else if (cmd == "text") {
		if (argc < 4) {
			std::cerr << "Params: [font file] [text file]!" << std::endl;
			return;
		} 

		edb::DistanceFieldFont dff(argv[2]);
		dff.test(argv[3]);
	} else if (cmd == "listfont") {
		if (argc < 5) {
			std::cerr << "Params: [font file] [text file] [out file]!" << std::endl;
			return;
		} 

		edb::DistanceFieldFont dff(argv[2]);
		dff.transLuaFileToList(argv[3], argv[4]);
	} else if (cmd == "loadtext") {
		if (argc < 3) {
			std::cerr << "Params: [font file]!" << std::endl;
			return;
		}

		edb::DFFParser parser(argv[2]);
		parser.outputImageFast(23, 25);

		//  		edb::DFFParser parser(argv[2]);
		//   		int edge = 8;
		//   		for (int i = 0; i < 8; ++i) {
		//   			parser.outputImageFast(edge, edge);
		//   			edge = edge << 1;
		//   		}
	} else if (cmd == "pack") {
		if (argc < 4) {
			std::cerr << "Params: [src dir] [dst file]!" << std::endl;
			return;
		}

		edb::PackParticle3D::Pack(argv[2], argv[3]);
	} else if (cmd == "clip") {
		if (argc < 3) {
			std::cerr << "Params: [dir] !" << std::endl;
			return;
		}

		edb::ClipImages::ClipImages(argv[2]);
	}
}

int main(int argc, char *argv[])
{
	InitSymbolCreators();

	try {
		command(argc, argv);
	} catch (d2d::Exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
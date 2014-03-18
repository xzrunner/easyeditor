#include <iostream>
#include <fstream>

#include "VerifyImages.h"
#include "VerifyJsons.h"
#include "FixImages.h"
#include "FixAnimation.h"
#include "FormatJsonFile.h"
#include "CopyFiles.h"

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

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "Need Command: [verify] ... !" << std::endl;
		std::cerr << "verify [folder path] (-images | -jsons)" << std::endl;
		return 1;
	}

	std::string cmd = argv[1];
	if (cmd == "verify")
	{
		if (argc < 3) {
			std::cerr << "Need Resources' folder path!" << std::endl;
			return 1;
		}

		std::string dirpath = argv[2];
		if (argc < 4) {
			std::cerr << "Need [-images] or [-jsons]!" << std::endl;
			return 1;
		}

		verify(dirpath, argv[3]);
		if (argc == 5)
			verify(dirpath, argv[4]);
	}
	else if (cmd == "fix")
	{
		if (argc < 5) {
			std::cerr << "Params: [path0] [path1] [-images] !" << std::endl;
			return 1;
		}

		fix(argv[2], argv[3], argv[4]);
	}
	else if (cmd == "format")
	{
		if (argc < 4) {
			std::cerr << "Params: [-jsons] [dir] !" << std::endl;
			return 1;
		}

		format(argv[3], argv[2]);
	}
	else if (cmd == "copy")
	{
		if (argc < 5) {
			std::cerr << "Params: [src] [dst] [list]!" << std::endl;
			return 1;
		}

		std::string srcdir = argv[2];
		std::string dstdir = argv[3];
		std::string listfile = argv[4];

		std::set<std::string> names;
		std::ifstream fin(listfile.c_str());
		if (fin.fail()) {
			std::cerr << "Can't open file " << listfile << std::endl;
			return 1;
		}
		std::string str;
		while (getline(fin, str)) {
			names.insert(str);
		}
		fin.close();

		edb::CopyFiles copy(srcdir, dstdir);
		copy.CopyByExportNames(names);
	}

	return 0;
}
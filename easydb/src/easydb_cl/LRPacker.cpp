#include "LRPacker.h"
#include "check_params.h"

#include "LRExpansion.h"
#include "SeparateToComplex.h"
#include "LRToComplex.h"
#include "LRLayersPack.h"
#include "PackCoco.h"

#include <wx/stdpaths.h>

namespace edb
{

std::string LRPacker::Command() const
{
	return "lr-packer";
}

std::string LRPacker::Description() const
{
	return "lr packer";
}

std::string LRPacker::Usage() const
{
	// lr-packer e:/test2/test_lr.json tmp_dir out_dir only_json
	std::string usage = Command() + " [filepath] [tmp dir] [out dir] [only json] [trim file]";
	return usage;
}

void LRPacker::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 6)) return;
	if (!check_file(argv[2])) return;
	if (!check_file(argv[6])) return;

	std::string tmp_dir = argv[3];
	std::string out_dir = argv[4];
	std::string tmp_lr_file = tmp_dir + "\\" + d2d::FilenameTools::getFilenameWithExtension(argv[2]);

	int id = d2d::StringTools::StringToInt(argv[5]);

	d2d::mk_dir(tmp_dir, true);
	d2d::mk_dir(out_dir, true);

	// 0
	LRExpansion exp;
	exp.Run(argv[2]);

	// 1
	SeparateToComplex sep;
	sep.Run(exp.GetOutputFilepath(argv[2]), "", tmp_lr_file);

	// 2
	LRToComplex tocomplex;
	tocomplex.Run(tmp_lr_file);	

	// 3
	LRLayersPack json_pack;
	json_pack.Run(tmp_lr_file);

	if (id == 1) {
		return;
	}

	// 4
	{
		Json::Value val;

		std::string trim_file = argv[6];
		trim_file = d2d::FilenameTools::getRelativePath(tmp_dir, trim_file);
		val["trim file"] = trim_file;

		Json::Value pkg_val;

		std::string lr_name = d2d::FilenameTools::getFilename(tmp_lr_file);
		lr_name = lr_name.substr(0, lr_name.find("_lr"));

		pkg_val["name"] = "scene_" + lr_name;

		int idx0 = 0;
		pkg_val["json list"][idx0] = lr_name + "_complex.json";

		std::string _out_dir = d2d::FilenameTools::getRelativePath(tmp_dir, out_dir);
		pkg_val["output dir"] = _out_dir;

		pkg_val["texture type"] = "png";

		val["packages"][idx0] = pkg_val;

		std::string cfg_file = tmp_dir + "\\config.json";
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(cfg_file.c_str());
		std::locale::global(std::locale("C"));
		writer.write(fout, val);
		fout.close();

		PackCoco pack;
		pack.Trigger(cfg_file);
	}

	// end
	wxRemoveFile(exp.GetOutputFilepath(argv[2]));
}

}
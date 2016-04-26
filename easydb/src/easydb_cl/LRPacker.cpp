#include "LRPacker.h"
#include "check_params.h"
#include "utility.h"
#include "lr_tools.h"

#include "LRExpandGroup.h"
#include "LRExpandInherit.h"
#include "LRSeparateComplex.h"
#include "LRToComplex.h"
#include "LRJsonPacker.h"
#include "PackRes.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/Config.h>

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
	// lr-packer e:/test2/test_lr.json tmp_dir out_dir only_json lod
	std::string usage = Command() + " [filepath] [tmp dir] [out dir] [only json] [LOD]";
	return usage;
}

int LRPacker::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 5)) return -1;
	if (!check_file(argv[2])) return -1;
//	if (!check_file(argv[6])) return;

	std::string tmp_dir = argv[3];
	std::string out_dir = argv[4];
	std::string tmp_lr_file = tmp_dir + "\\" + ee::FileHelper::GetFilenameWithExtension(argv[2]);

	bool only_json = ee::StringHelper::FromString<bool>(argv[5]);

	ee::FileHelper::MkDir(tmp_dir, true);
	ee::FileHelper::MkDir(out_dir, true);

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	ee::Config::Instance()->EnableRender(true);

	// 0
	LRExpandGroup ex_group;
	ex_group.Run(argv[2]);
	std::string group_outfile = ex_group.GetOutputFilepath(argv[2]);

	// 1
	LRExpandInherit ex_inherit;
	ex_inherit.Run(group_outfile);
	std::string file_outfile = ex_inherit.GetOutputFilepath(group_outfile);

	// 2
	LRSeparateComplex sep;
	sep.Run(file_outfile, "", tmp_lr_file);

	// 3
	LRToComplex tocomplex;
	tocomplex.Run(tmp_lr_file);	

	// 4
	LRJsonPacker json_pack;
	json_pack.Run(tmp_lr_file);

	// 5
	if (only_json != 1) {
		int LOD = ee::StringHelper::FromString<int>(argv[6]);
		PackEP(tmp_dir, tmp_lr_file, out_dir, LOD);
	}

	// end
	wxRemoveFile(group_outfile);
	wxRemoveFile(file_outfile);

	return 0;
}

void LRPacker::PackEP(const std::string& tmp_dir, const std::string& tmp_lr_file,
					  const std::string& out_dir, int LOD)
{
	Json::Value val;

	// 		std::string trim_file = argv[6];
	// 		trim_file = ee::FileHelper::GetRelativePath(tmp_dir, trim_file);
	// 		val["trim file"] = trim_file;

	Json::Value pkg_val;

	std::string lr_name = get_lr_name_from_file(tmp_lr_file);

	pkg_val["name"] = lr_name + "_scene";

	int idx = 0;
	pkg_val["json list"][idx] = lr_name + "_base_complex.json";
	++idx;
	pkg_val["json list"][idx] = lr_name + "_top_complex.json";
	++idx;
	{
		int i = 0;
		std::string filename = "name_" + ee::StringHelper::ToString(i) + "_complex.json";
		std::string dir = ee::FileHelper::GetFileDir(tmp_lr_file);
		while (ee::FileHelper::IsFileExist(dir + "\\" + filename)) {
			pkg_val["json list"][idx++] = filename;
			++i;
			filename = "name_" + ee::StringHelper::ToString(i) + "_complex.json";
		}
	}

	// 		int idx = 0;
	// 		pkg_val["json list"][idx] = ".";

	std::string _out_dir = ee::FileHelper::GetRelativePath(tmp_dir, out_dir);
	pkg_val["output dir"] = _out_dir;

	pkg_val["texture type"] = "png";
	pkg_val["tp extrude"] = 4;

	pkg_val["LOD"] = LOD;

	idx = 0;
	val["packages"][idx] = pkg_val;

	std::string cfg_file = tmp_dir + "\\config.json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(cfg_file.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, val);
	fout.close();

	PackRes pack;
	pack.Trigger(cfg_file);
}

}
#include "FixPackCfg.h"
#include "check_params.h"

#include <fstream>

namespace edb
{

std::string FixPackCfg::Command() const
{
	return "fix-pack-cfg";
}

std::string FixPackCfg::Description() const
{
	return "fix-pack-cfg";
}

std::string FixPackCfg::Usage() const
{
	return "fix-pack-cfg";
}

int FixPackCfg::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	Fix(argv[2]);
	return 0;
}

void FixPackCfg::Fix(const std::string& filepath)
{
	Json::Value val;

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	Json::Value fixed = val;

	for (int i = 0, n = val.size(); i < n; ++i)
	{
		const Json::Value& cval = val[i];
		int pkg_id = cval["pkg_id"].asInt();
		if (pkg_id < 2000) {
			continue;
		}
		Json::Value new_val = cval;
		new_val["pkg_id"] = pkg_id + 1;
		new_val["name"] = cval["name"].asString() + "_base";
		fixed[fixed.size()] = new_val;		
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, fixed);
	fout.close();
}

}
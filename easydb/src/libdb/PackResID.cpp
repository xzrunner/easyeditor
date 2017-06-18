#include "PackResID.h"
#include "check_params.h"

#include <gum/FilepathHelper.h>

#include <easysprpacker.h>
#include <easyrespacker.h>

#include <fstream>

#include <stdint.h>

namespace edb
{

std::string PackResID::Command() const
{
	return "pack-res-id";
}

std::string PackResID::Description() const
{
	return "pack res id";
}

std::string PackResID::Usage() const
{
	return Command() + " [src dir] [dst dir]";
}

int PackResID::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

	Pack(argv[2], argv[3]);

	return 0;
}

void PackResID::Pack(const std::string& src, const std::string& dst)
{
	std::string pkg_src_file = gum::FilepathHelper::Absolute(src, "pkg_cfg.json");
	std::string pkg_dst_file = gum::FilepathHelper::Absolute(dst, "pkg.bin");	
	LoadPkgID(pkg_src_file);
	StorePkgID(pkg_dst_file);
}

void PackResID::LoadPkgID(const std::string& src)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(src.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	for (int i = 0, n = val.size(); i < n; ++i)
	{
		Json::Value pval = val[i];
		PkgID p;
		p.name = pval["name"].asString();
		p.path = pval["path"].asString();
		if (p.path.size() > 3) {
			p.path = p.path.substr(3);
		}
		p.id = pval["pkg_id"].asInt();
		m_pkgs.push_back(p);
	}
}

void PackResID::StorePkgID(const std::string& dst) const
{
	// size
	int sz = 0;
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_pkgs.size(); i < n; ++i) {
		sz += esprpacker::sizeof_pack_str(m_pkgs[i].name);
		sz += esprpacker::sizeof_pack_str(m_pkgs[i].path);
		sz += sizeof(uint16_t);
	}

	// filling
	uint8_t* buf = new uint8_t[sz];
	uint8_t* ptr = buf;
	uint16_t n = m_pkgs.size();
	esprpacker::pack(n, &ptr);
	for (int i = 0; i < n; ++i) {
		esprpacker::pack_str(m_pkgs[i].name, &ptr);
		esprpacker::pack_str(m_pkgs[i].path, &ptr);
		uint16_t id = m_pkgs[i].id;
		esprpacker::pack(id, &ptr);
	}

	// output
	std::locale::global(std::locale(""));
	std::ofstream fout(dst.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	
	uint8_t* dst = NULL;
	size_t dst_sz;
	erespacker::Lzma::Compress(&dst, &dst_sz, buf, sz);
	if (dst_sz > 0) {
		fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(dst_sz));
		fout.write(reinterpret_cast<const char*>(dst), dst_sz);
	}
	delete[] buf;
	fout.close();
}

}
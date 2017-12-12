#include "PackPkg.h"
#include "check_params.h"

#include <ee/Exception.h>

#include <gum/FilepathHelper.h>
#include <gum/StringHelper.h>
#include <bimp/ImportStream.h>
#include <simp/PkgIdxLoader.h>

#include <fstream>

namespace edb
{

PackPkg::PackPkg()
	: m_buf(NULL)
	, m_buf_size(0)
{
}

PackPkg::~PackPkg()
{
	delete[] m_buf;
}

std::string PackPkg::Command() const
{
	return "pack-pkg";
}

std::string PackPkg::Description() const
{
	return "pack multi ep files to one pkg file";
}

std::string PackPkg::Usage() const
{
	return Command() + " [dir] [name]";
}

int PackPkg::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void PackPkg::Trigger(const std::string& dir, const std::string& name)
{
	auto epe_path = gum::FilepathHelper::Absolute(dir.c_str(), CU_STR(name.c_str()) + ".epe");
	if (!gum::FilepathHelper::Exists(epe_path)) {
		throw ee::Exception("no epe file %s", epe_path.c_str());
	}
	auto ept_path = gum::FilepathHelper::Absolute(dir.c_str(), CU_STR(name.c_str()) + ".ept");
	if (!gum::FilepathHelper::Exists(ept_path)) {
		throw ee::Exception("no ept file %s", ept_path.c_str());
	}

	CU_MAP<CU_STR, uint32_t> export_names;
	CU_VEC<simp::Package::PageDesc> pages;
	CU_VEC<int> ref_pkgs;
	simp::PkgIdxLoader epe_loader(epe_path.c_str(), export_names, pages, ref_pkgs);
	epe_loader.Load();
	int epe_num = pages.size();

	EptLoader ept_loader(ept_path.c_str());
	ept_loader.Load();
	int ept_num = ept_loader.GetCount();

	std::vector<uint32_t> epe_size, ept_size;
	epe_size.resize(epe_num + 1);
	ept_size.resize(ept_num + 1);

	epe_size[0] = GetFileSize(epe_path.c_str());
	for (int i = 0; i < epe_num; ++i) 
	{
		auto filename = CU_STR(name.c_str()) + "." + gum::StringHelper::ToString(i + 1) + ".epe";
		auto filepath = gum::FilepathHelper::Absolute(dir.c_str(), filename);
		if (!gum::FilepathHelper::Exists(filepath)) {
			throw ee::Exception("no epe file %s", filepath.c_str());
		}
		epe_size[i + 1] = GetFileSize(filepath.c_str());
	}

	ept_size[0] = GetFileSize(ept_path.c_str());
	for (int i = 0; i < ept_num; ++i)
	{
		auto filename = CU_STR(name.c_str()) + "." + gum::StringHelper::ToString(i + 1) + ".ept";
		auto filepath = gum::FilepathHelper::Absolute(dir.c_str(), filename);
		if (!gum::FilepathHelper::Exists(filepath)) {
			throw ee::Exception("no ept file %s", filepath.c_str());
		}
		ept_size[i + 1] = GetFileSize(filepath.c_str());
	}

	//if (epe_size.size() <= 1 || ept_size.size() <= 1) {
	//	throw ee::Exception("less ep file dir: %s, name: %s, epe %d, ept %d",
	//		dir.c_str(), name.c_str(), epe_size.size(), ept_size.size());
	//}

	auto pkg_path = gum::FilepathHelper::Absolute(dir.c_str(), CU_STR(name.c_str()) + ".pkg");
	std::locale::global(std::locale(""));
	std::ofstream fout(pkg_path.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	

	// header
	uint16_t count = epe_size.size();
	fout.write(reinterpret_cast<const char*>(&count), sizeof(count));
	count = ept_size.size();
	fout.write(reinterpret_cast<const char*>(&count), sizeof(count));
	//
	uint32_t data_head = sizeof(uint16_t) * 2 + sizeof(uint32_t) * (epe_size.size() + ept_size.size());
	uint32_t ptr = data_head;
	// epe header
	for (int i = 0, n = epe_size.size(); i < n; ++i) 
	{
		uint32_t size = epe_size[i];
		fout.write(reinterpret_cast<const char*>(&ptr), sizeof(ptr));
		ptr += size;
	}
	// ept header
	for (int i = 0, n = ept_size.size(); i < n; ++i) 
	{
		uint32_t size = ept_size[i];
		fout.write(reinterpret_cast<const char*>(&ptr), sizeof(ptr));
		ptr += size;
	}
	// epe data
	epe_path = gum::FilepathHelper::Absolute(dir.c_str(), CU_STR(name.c_str()) + ".epe");
	WriteFile(epe_path.c_str(), epe_size[0], fout);
	for (int i = 1, n = epe_size.size(); i < n; ++i) 
	{
		auto filename = CU_STR(name.c_str()) + "." + gum::StringHelper::ToString(i) + ".epe";
		auto filepath = gum::FilepathHelper::Absolute(dir.c_str(), filename);
		WriteFile(filepath.c_str(), epe_size[i], fout);
	}
	// ept data
	ept_path = gum::FilepathHelper::Absolute(dir.c_str(), CU_STR(name.c_str()) + ".ept");
	WriteFile(ept_path.c_str(), ept_size[0], fout);
	for (int i = 1, n = ept_size.size(); i < n; ++i) 
	{
		auto filename = CU_STR(name.c_str()) + "." + gum::StringHelper::ToString(i) + ".ept";
		auto filepath = gum::FilepathHelper::Absolute(dir.c_str(), filename);
		WriteFile(filepath.c_str(), ept_size[i], fout);
	}

	fout.close();
}

uint32_t PackPkg::GetFileSize(const std::string& filepath)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	fin.seekg(0, std::ios::end);
	uint32_t size = fin.tellg();
	fin.close();
	return size;
}

void PackPkg::WriteFile(const std::string& src, uint32_t size, std::ofstream& fout)
{
	if (m_buf_size < size) 
	{
		delete[] m_buf;
		m_buf = new unsigned char[size * 2];
		m_buf_size = size * 2;
	}

	std::locale::global(std::locale(""));
	std::ifstream fin(src.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	fin.read(reinterpret_cast<char*>(m_buf), size);
	fout.write(reinterpret_cast<const char*>(m_buf), size);
	fin.close();
}

/************************************************************************/
/* class PackPkg::EptLoader                                             */
/************************************************************************/

PackPkg::EptLoader::EptLoader(const CU_STR& filepath)
	: bimp::FileLoader(filepath)
{
}

int PackPkg::EptLoader::GetCount() const
{
	return m_count;
}

void PackPkg::EptLoader::OnLoad(bimp::ImportStream& is)
{
	m_count = is.UInt16();
}

}
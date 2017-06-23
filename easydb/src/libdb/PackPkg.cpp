#include "PackPkg.h"
#include "check_params.h"

#include <ee/Exception.h>

#include <gum/FilepathHelper.h>
#include <gum/StringHelper.h>

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
	std::vector<uint32_t> epe_size, ept_size;

	std::string filepath = gum::FilepathHelper::Absolute(dir, name + ".epe");
	if (!gum::FilepathHelper::Exists(filepath)) {
		throw ee::Exception("no epe file %s", filepath.c_str());
	}
	epe_size.push_back(GetFileSize(filepath));
	int i = 1;
	while (true)
	{
		std::string filename = name + "." + gum::StringHelper::ToString(i) + ".epe";
		std::string filepath = gum::FilepathHelper::Absolute(dir, filename);
		if (!gum::FilepathHelper::Exists(filepath)) {
			break;
		} else {
			epe_size.push_back(GetFileSize(filepath));
		}
		++i;
	}

	filepath = gum::FilepathHelper::Absolute(dir, name + ".ept");
	if (!gum::FilepathHelper::Exists(filepath)) {
		throw ee::Exception("no ept file %s", filepath.c_str());
	}
	ept_size.push_back(GetFileSize(filepath));
	i = 1;
	while (true)
	{
		std::string filename = name + "." + gum::StringHelper::ToString(i) + ".ept";
		std::string filepath = gum::FilepathHelper::Absolute(dir, filename);
		if (!gum::FilepathHelper::Exists(filepath)) {
			break;
		} else {
			ept_size.push_back(GetFileSize(filepath));
		}
		++i;
	}

	if (epe_size.size() <= 1 || ept_size.size() <= 1) {
		throw ee::Exception("less ep file dir: %s, name: %s, epe %d, ept %d",
			dir.c_str(), name.c_str(), epe_size.size(), ept_size.size());
	}

	filepath = gum::FilepathHelper::Absolute(dir, name + ".pkg");
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	
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
	filepath = gum::FilepathHelper::Absolute(dir, name + ".epe");
	WriteFile(filepath, epe_size[0], fout);
	for (int i = 1, n = epe_size.size(); i < n; ++i) 
	{
		std::string filename = name + "." + gum::StringHelper::ToString(i) + ".epe";
		std::string filepath = gum::FilepathHelper::Absolute(dir, filename);
		WriteFile(filepath, epe_size[i], fout);
	}
	// ept data
	filepath = gum::FilepathHelper::Absolute(dir, name + ".ept");
	WriteFile(filepath, ept_size[0], fout);
	for (int i = 1, n = ept_size.size(); i < n; ++i) 
	{
		std::string filename = name + "." + gum::StringHelper::ToString(i) + ".ept";
		std::string filepath = gum::FilepathHelper::Absolute(dir, filename);
		WriteFile(filepath, ept_size[i], fout);
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

}
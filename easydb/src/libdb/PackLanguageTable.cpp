#include "PackLanguageTable.h"
#include "check_params.h"

#include <easysprpacker.h>
#include <easyrespacker.h>

#include <cu/cu_stl.h>
#include <gum/StringHelper.h>
#include <playdb/storage/DiskStorageManager.h>
#include <playdb/btree/BTree.h>

#include <fstream>

#include <assert.h>

namespace edb
{

std::string PackLanguageTable::Command() const
{
	return "pack-lang";
}

std::string PackLanguageTable::Description() const
{
	return "pack language";
}

std::string PackLanguageTable::Usage() const
{
	std::string usage = Command() + " [src file] [dst file]";
	return usage;
}

int PackLanguageTable::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_file(argv[2])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void PackLanguageTable::Trigger(const std::string& src_file, const std::string& dst_file)
{
	LoadFromCSV(src_file);

	PackToBin(dst_file);
//	PackToPlayDB(dst_file);
}

void PackLanguageTable::LoadFromCSV(const std::string& filepath)
{
	m_header.clear();
	m_body.clear();

	std::ifstream fin(filepath.c_str());

	// header
	CU_STR str_line;
	getline(fin, str_line);
	gum::StringHelper::Split(str_line, ",", m_header);
	int language_num = m_header.size() - 1;
	for (auto& header : m_header) {
		header = gum::StringHelper::GBKToUTF8(header.c_str());
	}

	// body
	while (getline(fin, str_line))
	{
		CU_VEC<CU_STR> items;
		gum::StringHelper::Split(str_line, ",", items);
		while (items.size() < language_num + 1) {
			items.push_back("");
		}
		assert(items.size() == language_num + 1);
		for (int i = 1, n = items.size(); i < n; ++i) {
			gum::StringHelper::Replace(items[i], "\\n", "\n");
		}
		for (auto& item : items) {
			item = gum::StringHelper::GBKToUTF8(item.c_str());
		}
		m_body.push_back(items);
	}

	fin.close();
}

void PackLanguageTable::PackToBin(const std::string& filepath)
{
	// size
	int sz = 0;
	sz += sizeof(uint16_t); // items count
	sz += sizeof(uint16_t); // types count
	assert(m_header.size() > 0);
	for (int i = 1, n = m_header.size(); i < n; ++i) {
		sz += esprpacker::sizeof_pack_str(m_header[i].c_str());
	}
	for (auto& item : m_body) {
		assert(item.size() == m_header.size());
		for (auto& type : item) {
			sz += esprpacker::sizeof_pack_long_str(type.c_str());
		}
	}

	// filling
	uint8_t* buf = new uint8_t[sz];
	uint8_t* ptr = buf;
	uint16_t items_count = m_body.size();
	esprpacker::pack(items_count, &ptr);
	uint16_t types_count = m_header.size() - 1;
	esprpacker::pack(types_count, &ptr);
	for (int i = 1, n = m_header.size(); i < n; ++i) {
		esprpacker::pack_str(m_header[i].c_str(), &ptr);
	}
	for (auto& item : m_body) {
		assert(item.size() == m_header.size());
		for (auto& type : item) {
			esprpacker::pack_long_str(type.c_str(), &ptr);
		}
	}

	// output
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
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

void PackLanguageTable::PackToPlayDB(const std::string& filepath)
{
	auto storage_mgr = std::make_unique<playdb::storage::DiskStorageManager>(
		filepath + ".idx", filepath + ".dat", true, 4096);
	playdb::btree::BTree<std::string> tree(storage_mgr.get(), 25);

	for (auto& entry : m_body)
	{
		size_t len = 0;
		len += sizeof(uint8_t);
		for (int i = 1, n = entry.size(); i < n; ++i) {
			len += esprpacker::sizeof_pack_long_str(entry[i].c_str());
		}

		uint8_t* data = new uint8_t[len];
		uint8_t* ptr = data;

		uint8_t sz = entry.size() - 1;
		esprpacker::pack(sz, &ptr);
		for (int i = 1, n = entry.size(); i < n; ++i) {
			esprpacker::pack_long_str(entry[i].c_str(), &ptr);
		}

		tree.InsertData(entry[0].c_str(), len, data);
	}
}

}
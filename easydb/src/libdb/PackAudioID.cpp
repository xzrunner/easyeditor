#include "PackAudioID.h"
#include "check_params.h"

#include <gum/FilepathHelper.h>

#include <easysprpacker.h>
#include <easyrespacker.h>

#include <fstream>

#include <stdint.h>

namespace edb
{

std::string PackAudioID::Command() const
{
	return "pack-audio-id";
}

std::string PackAudioID::Description() const
{
	return "pack audio id";
}

std::string PackAudioID::Usage() const
{
	return Command() + " [src file] [dst file]";
}

int PackAudioID::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_file(argv[2])) return -1;

	Pack(argv[2], argv[3]);

	return 0;
}

void PackAudioID::Pack(const std::string& src, const std::string& dst)
{
	LoadAudioID(src);
	StoreAudioID(dst);
}

void PackAudioID::LoadAudioID(const std::string& src)
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
		Json::Value aval = val[i];
		AudioID a;
		a.name = aval["name"].asString();
		a.id = aval["id"].asInt();
		m_audios.push_back(a);
	}
}

void PackAudioID::StoreAudioID(const std::string& dst_str) const
{
	// size
	int sz = 0;
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_audios.size(); i < n; ++i) {
		sz += esprpacker::sizeof_pack_str(m_audios[i].name);
		sz += sizeof(uint32_t);
	}

	// filling
	uint8_t* buf = new uint8_t[sz];
	uint8_t* ptr = buf;
	uint16_t n = m_audios.size();
	esprpacker::pack(n, &ptr);
	for (int i = 0; i < n; ++i) {
		esprpacker::pack_str(m_audios[i].name, &ptr);
		uint32_t id = m_audios[i].id;
		esprpacker::pack(id, &ptr);
	}

	// output
	std::locale::global(std::locale(""));
	std::ofstream fout(dst_str.c_str(), std::ios::binary);
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
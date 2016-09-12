#ifndef _EASYSPRPACKER_PACKER_H_
#define _EASYSPRPACKER_PACKER_H_

#include <ee/TexturePacker.h>

namespace ee { class Symbol; }

namespace esprpacker
{

class Packer
{
public:
	Packer(const std::string& json_dir, const std::string& tp_name, 
		const std::string& tp_dir);
	~Packer();

	void OutputLua(const std::string& outfile, float scale = 1) const;

	void OutputEpe(const std::string& outfile, bool compress, float scale = 1) const;
	void OutputEpt(const std::string& outfile, int LOD = 0, float scale = 1) const;

private:
	void LoadJsonData(const std::string& dir);
	void LoadTPData(const std::string& tp_name);

	void Pack() const;

	void OutputEptDesc(const std::string& outfile) const;

private:
	std::vector<const ee::Symbol*> m_syms;

	ee::TexturePacker m_tp;

}; // Packer

}

#endif // _EASYSPRPACKER_PACKER_H_
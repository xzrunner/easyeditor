#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <wx/wx.h>

namespace edb {

class VerifyImages
{
public:
	VerifyImages(const std::string& dirpath);

private:
	void InitFiles(const std::string& dirpath);

	void VerifyLack();
	void VerifySurplus();
	void Report() const;

	void HandleSpritePath(const std::string& filepath);

private:
	std::vector<std::string> _complex_files;
	std::vector<std::string> _anim_files;
	std::map<std::string, bool> _map_images;

	std::set<std::string> _reports;

}; // VerifyImages

}
#ifndef _DRAG2D_CONFIG_H_
#define _DRAG2D_CONFIG_H_

#include <JSON/json.h>

namespace d2d
{

class Config
{
public:
	static Config* Instance();

	bool IsUseDTex() const { return m_use_dtex; }

	const std::string& GetFontFilepath() const { return m_font_filepath; }

	void GetStrings(const std::string& key, std::vector<std::string>& val) const;

private:
	Config();

	void LoadFromFile(const char* filename);

private:
	Json::Value m_value;

	bool m_use_dtex;

	std::string m_font_filepath;

private:
	static Config* m_instance;

}; // Config

}

#endif // _DRAG2D_CONFIG_H_
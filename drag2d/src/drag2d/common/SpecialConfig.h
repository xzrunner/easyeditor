#ifndef _D2D_SPECIAL_CONFIG_H_
#define _D2D_SPECIAL_CONFIG_H_

#include <string>
#include <json/json.h>

namespace d2d
{

class SpecialConfig
{
public:
	struct Window
	{
		int width, height;
		int left, top;
	};

public:
	SpecialConfig(const std::string& file_tag);
	~SpecialConfig();

	bool IsExist() const {
		return m_exist;
	}

	const Window& GetWnd() const {
		return m_wnd;
	}
	void SetWnd(const Window& wnd) {
		m_wnd = wnd;
	}

private:
	void Load();
	void Store() const;

private:
	std::string m_filename;

	mutable Json::Value m_value;

	bool m_exist;	

	Window m_wnd;

	bool m_auto_set_wnd;

}; // SpecialConfig

}

#endif // _D2D_SPECIAL_CONFIG_H_

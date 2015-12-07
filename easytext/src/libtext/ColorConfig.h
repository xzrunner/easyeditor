#ifndef _EASYTEXT_COLOR_CONFIG_H_
#define _EASYTEXT_COLOR_CONFIG_H_

#include <wx/colordlg.h>

namespace etext
{

class ColorConfig
{
public:
	wxColourData* GetColorData() { return &m_color_data; }
	void SetColorData(const wxColourData& color_data) { m_color_data = color_data; }

	void StoreToFile() const;
	void LoadFromFile();

	static ColorConfig* Instance();

private:
	ColorConfig() {}

private:
	wxColourData m_color_data;

private:
	static ColorConfig* m_instance;
 
}; // ColorConfig

}

#endif // _EASYTEXT_COLOR_CONFIG_H_
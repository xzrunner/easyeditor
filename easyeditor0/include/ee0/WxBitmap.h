#pragma once

#include <wx/bitmap.h>

#include <string>
#include <memory>

namespace ee0
{

class WxBitmap
{
public:
	WxBitmap(const std::string& filepath);

	const std::unique_ptr<wxBitmap>& GetLargeBmp() const { return m_bmp_large; }
	const std::unique_ptr<wxBitmap>& GetSmallBmp() const { return m_bmp_small; }

private:
	bool LoadFromFile(const std::string& filepath);

	void InitBmp(const wxImage& image, bool scale);

	static void LoadWXImage(const std::string& filepath, wxImage& image);

private:
	std::unique_ptr<wxBitmap> m_bmp_large = nullptr;
	std::unique_ptr<wxBitmap> m_bmp_small = nullptr;

}; // WxBitmap

}
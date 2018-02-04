#pragma once

#include <string>
#include <memory>

namespace ee0
{

class WxBitmap;

class WxLibraryItem
{
public:
	WxLibraryItem(const std::string& filepath);

	const std::string& GetFilepath() const { return m_filepath; }

	const std::string& GetName() const { return m_name; }
	const std::string& GetInfo() const { return m_info; }

	const std::shared_ptr<WxBitmap>& GetBitmap() const {
		return m_bitmap;
	}

private:
	std::string m_filepath;

	std::string m_name;
	std::string m_info;

	std::shared_ptr<WxBitmap> m_bitmap = nullptr;

}; // WxLibraryItem

}
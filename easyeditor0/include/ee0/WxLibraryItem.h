#pragma once

#include <string>
#include <memory>

namespace s2 { class Symbol; }

namespace ee0
{

class WxBitmap;

class WxLibraryItem
{
public:

	void Refresh(const s2::Symbol& sym, bool force = false);

	const std::string& GetName() const { return m_name; }
	const std::string& GetInfo() const { return m_info; }

	const std::shared_ptr<WxBitmap>& GetBitmap() const {
		return m_bitmap;
	}

private:
	std::string m_name;
	std::string m_info;

	std::shared_ptr<WxBitmap> m_bitmap = nullptr;

}; // WxLibraryItem

}
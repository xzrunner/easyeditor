#include "ee0/WxLibraryItem.h"
#include "ee0/WxBitmap.h"

#include <boost/filesystem.hpp>

namespace ee0
{

WxLibraryItem::WxLibraryItem(const std::string& filepath)
	: m_filepath(filepath)
{
	m_name = boost::filesystem::path(filepath).filename().string();

	m_bitmap = std::make_shared<WxBitmap>(filepath);
}

}
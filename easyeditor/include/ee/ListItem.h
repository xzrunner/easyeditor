#ifndef _EASYEDITOR_LIST_ITEM_H_
#define _EASYEDITOR_LIST_ITEM_H_

#include "Bitmap.h"

#include <string>

namespace ee
{

class ListItem
{
public:
	ListItem() : m_bitmap(NULL) {}
	virtual ~ListItem() {}

	void RefreshThumbnail(const std::string& filepath, bool force = false);

	const std::string& GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	const std::string& GetInfo() const { return m_info; }
	void SetInfo(const std::string& info) { m_info = info; }

	const BitmapPtr& GetBitmap() const { return m_bitmap; }
	void SetBitmap(const BitmapPtr& bmp);

protected:
	std::string m_name;
	std::string m_info;

	BitmapPtr m_bitmap;

}; // ListItem

using ListItemPtr = std::shared_ptr<ListItem>;

}

#endif // _EASYEDITOR_LIST_ITEM_H_
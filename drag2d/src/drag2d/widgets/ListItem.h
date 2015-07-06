#ifndef _DRAG2D_LIST_ITEM_H_
#define _DRAG2D_LIST_ITEM_H_

#include "common/Object.h"
#include "interfaces.h"

namespace d2d
{

class Bitmap;

class ListItem : public Object, public ICloneable
{
public:
	ListItem() {
		m_bitmap = NULL;
	}
	virtual ~ListItem() {}

	void RefreshThumbnail(const std::string& filepath, bool force = false);

	const std::string& GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	const std::string& GetInfo() const { return m_info; }
	void SetInfo(const std::string& info) { m_info = info; }

	const Bitmap* GetBitmap() const { return m_bitmap; }

protected:
	std::string m_name;
	std::string m_info;

	Bitmap* m_bitmap;

}; // ListItem

}

#endif // _DRAG2D_LIST_ITEM_H_
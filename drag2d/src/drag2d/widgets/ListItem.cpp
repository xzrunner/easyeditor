#include "ListItem.h"

#include "dataset/Bitmap.h"

namespace d2d
{

void ListItem::RefreshThumbnail(const wxString& filepath, bool force)
{
	if (m_bitmap) {
		if (force) {
			m_bitmap->release();
		} else {
			return;
		}
	}
	m_bitmap = BitmapMgr::Instance()->getItem(filepath);
	m_bitmap->loadFromFile(filepath);
}

}
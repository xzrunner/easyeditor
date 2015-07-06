#include "ListItem.h"

#include "dataset/Bitmap.h"

namespace d2d
{

void ListItem::RefreshThumbnail(const std::string& filepath, bool force)
{
	if (m_bitmap) {
		if (force) {
			m_bitmap->Release();
		} else {
			return;
		}
	}
	m_bitmap = BitmapMgr::Instance()->GetItem(filepath);
	m_bitmap->LoadFromFile(filepath);
}

}
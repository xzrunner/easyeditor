#include "ListItem.h"
#include "Bitmap.h"
#include "Symbol.h"

namespace ee
{

void ListItem::RefreshThumbnail(const std::string& filepath, bool force)
{
	if (m_bitmap) {
		if (force) {
			m_bitmap->RemoveReference();
		} else {
			return;
		}
	}
	m_bitmap = BitmapMgr::Instance()->GetItem(filepath);
	if (!m_bitmap) {
		Symbol* sym = dynamic_cast<Symbol*>(this);
		if (sym) {
			m_bitmap = new Bitmap(sym);
		}
	}
}

}
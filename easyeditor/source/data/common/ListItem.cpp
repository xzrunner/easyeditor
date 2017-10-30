#include "ListItem.h"
#include "Symbol.h"
#include "Bitmap.h"

#include <sprite2/SymType.h>

namespace ee
{

void ListItem::RefreshThumbnail(const std::string& filepath, bool force)
{
	if (!force && m_bitmap) {
		return;
	}

	Symbol* sym = dynamic_cast<Symbol*>(this);
	if (!sym || sym->Type() == s2::SYM_IMAGE || sym->Type() == s2::SYM_AUDIO) {
		return;
	}

	m_bitmap = std::make_shared<Bitmap>(*sym);
}

void ListItem::SetBitmap(const BitmapPtr& bmp)
{
	m_bitmap = bmp;
}

}
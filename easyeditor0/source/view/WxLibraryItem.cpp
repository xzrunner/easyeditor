#include "ee0/WxLibraryItem.h"
#include "ee0/CompSymEditor.h"
#include "ee0/WxBitmap.h"

#include <sprite2/Symbol.h>
#include <sprite2/SymType.h>

namespace ee0
{

void WxLibraryItem::Refresh(const s2::Symbol& sym, bool force)
{
	if (!force && m_bitmap) {
		return;
	}

	if (sym.Type() == s2::SYM_IMAGE || sym.Type() == s2::SYM_AUDIO) {
		return;
	}

	auto& ceditor = sym.GetComponent<CompSymEditor>();
	m_bitmap = std::make_shared<WxBitmap>(ceditor.GetFilepath());
}

}
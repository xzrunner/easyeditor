#include "SkeletonSymbolList.h"
#include "LibraryPanel.h"
#include "SkeletonEditPanel.h"
#include "dataset/Symbol.h"

#include <wx/statline.h>

namespace eanim
{

SkeletonSymbolList::SkeletonSymbolList(wxWindow* parent, LibraryPanel* library)
	: ee::VerticalImageList(parent, SkeletonEditPanel::BONE_SYMBOL_LIST_NAME)
{
	int index = 0;
	while (true)
	{
		Symbol* symbol = library->getSymbol(index++);
		if (!symbol) break;

		if (symbol->getSkeletonBody())
			insert(symbol);
	}
}

void SkeletonSymbolList::reloadAllTexture()
{
	for (size_t i = 0, n = m_items.size(); i < n; ++i)
		static_cast<Symbol*>(m_items[i])->reloadTexture();
}

Symbol* SkeletonSymbolList::getSymbol(int index) const
{
	if (index < 0 || index >= m_items.size()) return NULL;
	else return static_cast<Symbol*>(m_items[index]);
}

} // eanim
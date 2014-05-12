#include "LibrarySymbolList.h"
#include "SymbolEditDialog.h"
#include "StagePanel.h"
#include "LibraryPanel.h"

#include "dataset/Symbol.h"
#include "frame/Context.h"

namespace eanim
{

LibrarySymbolList::LibrarySymbolList(wxWindow* parent)
	: d2d::LibraryList(parent, LibraryPanel::SYMBOL_LIST_NAME)
{
}

void LibrarySymbolList::loadFromTextFile(std::ifstream& fin)
{
	clear();

	std::string flag;
	size_t size;
	fin >> flag >> size;
	for (size_t i = 0; i < size; ++i)
	{
		Symbol* symbol = new Symbol;
		symbol->loadFromTextFile(fin);
		insert(symbol);
	}
}

void LibrarySymbolList::storeToTextFile(std::ofstream& fout) const
{
	fout << "symbol " << m_items.size() << '\n';
	for (size_t i = 0, n = m_items.size(); i < n; ++i)
	{
		Symbol* symbol = static_cast<Symbol*>(m_items[i]);
		symbol->storeToTextFile(fout);
	}
}

void LibrarySymbolList::onListDoubleClicked(wxCommandEvent& event)
{
	SymbolEditDialog dlg(this, static_cast<Symbol*>(m_items[event.GetInt()]));
	dlg.ShowModal();
	Context::Instance()->stage->resetCanvas();
}

} // eanim
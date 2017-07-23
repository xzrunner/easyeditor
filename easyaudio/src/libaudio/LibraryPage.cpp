#include "LibraryPage.h"
#include "Symbol.h"

#include <ee/LibraryList.h>

#include <sprite2/SymType.h>

namespace eaudio
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Audio")
{
	InitLayout();
	m_list->SetFileter("mp3");
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return sym->Type() == s2::SYM_AUDIO;
}

}
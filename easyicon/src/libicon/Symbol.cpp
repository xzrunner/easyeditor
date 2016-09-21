#include "Symbol.h"
#include "Icon.h"
#include "FileIO.h"

#include <ee/ImageSymbol.h>

namespace eicon
{

void Symbol::SetImage(ee::ImageSymbol* img)
{
	if (m_icon) {
		m_icon->SetImage(img);
	}
}

void Symbol::LoadResources()
{
	Icon* icon = FileIO::LoadFromFile(m_filepath.c_str());
	SetIcon(icon);
	icon->RemoveReference();
}

}
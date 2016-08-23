#include "Symbol.h"
#include "Icon.h"
#include "FileIO.h"

namespace eicon
{

void Symbol::SetImage(ee::Image* img)
{
	if (m_icon) {
		dynamic_cast<Icon*>(m_icon)->SetImage(img);
	}
}

void Symbol::LoadResources()
{
	Icon* icon = FileIO::LoadFromFile(m_filepath.c_str());
	SetIcon(icon);
	icon->RemoveReference();
}

}
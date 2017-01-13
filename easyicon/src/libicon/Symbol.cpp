#include "Symbol.h"
#include "Icon.h"
#include "FileIO.h"

#include <ee/ImageSymbol.h>

#include <gum/FilepathHelper.h>

namespace eicon
{

void Symbol::SetImage(ee::ImageSymbol* img)
{
	if (m_icon) {
		m_icon->SetImage(img);
	}
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	Icon* icon = FileIO::LoadFromFile(m_filepath.c_str());
	SetIcon(icon);
	icon->RemoveReference();

	return true;
}

}
#include "TopPannels.h"

#include "view/LibraryPanel.h"
#include "view/StagePanel.h"

namespace eui
{

TopPannels::TopPannels()
{
	library = NULL;
	toolbar = NULL;
}

void TopPannels::Clear()
{
	library->Clear();
	stage->Clear();
}

}
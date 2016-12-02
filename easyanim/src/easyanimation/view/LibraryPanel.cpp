#include "LibraryPanel.h"

#include "view/ViewMgr.h"

#include <ee/LibraryImagePage.h>

#include <easycomplex.h>
#include <easyskeleton.h>
#include <easyanim.h>
#include <easymesh.h>
#include <easymask.h>
#include <easyscale9.h>
#include <easyicon.h>
#include <easyparticle3d.h>
#include <easytrail.h>

namespace eanim
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(ViewMgr::Instance()->img_page = new ee::LibraryImagePage(nb));
	AddPage(new libskeleton::LibraryPage(nb));	
	AddPage(new ecomplex::LibraryPage(nb));
	AddPage(new libanim::LibraryPage(nb));
	AddPage(new emesh::LibraryPage(nb));
	AddPage(new emask::LibraryPage(nb));
	AddPage(new escale9::LibraryPage(nb));
	AddPage(new eicon::LibraryPage(nb));
	AddPage(new eparticle3d::LibraryPage(nb));
	AddPage(new etrail::LibraryPage(nb));	
}

}
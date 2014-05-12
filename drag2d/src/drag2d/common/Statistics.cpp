#include "Statistics.h"

#include "dataset/SymbolMgr.h"
#include "dataset/BodyData.h"
#include "dataset/Bitmap.h"
#include "dataset/Font.h"
#include "dataset/Image.h"

namespace d2d
{

void Statistics::PrintRes()
{
	wxString msg;
	msg += "Symbol: " + wxString::FromDouble(SymbolMgr::Instance()->size()) + "\n";
	msg += "Body: " + wxString::FromDouble(BodyDataMgr::Instance()->size()) + "\n";
	msg += "Bitmap: " + wxString::FromDouble(BitmapMgr::Instance()->size()) + "\n";
	msg += "Font: " + wxString::FromDouble(FontMgr::Instance()->size()) + "\n";
	msg += "Image: " + wxString::FromDouble(ImageMgr::Instance()->size()) + "\n";
	wxMessageBox(msg, wxT("Statistics"), wxOK | wxICON_INFORMATION/*, this*/);
}

}